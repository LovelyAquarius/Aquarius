#pragma once
#include<IMGUI/imgui.h>
#include<imgui_internal.h>


namespace Aquarius
{
    using namespace ImGui;
    static ImGuiDockNode* DockContextAddNode(ImGuiContext* ctx, ImGuiID id)
    {
        // Generate an ID for the new node (the exact ID value doesn't matter as long as it is not already used) and add the first window.
        ImGuiContext& g = *ctx;
        if (id == 0)
            id = DockContextGenNodeID(ctx);
        else
            IM_ASSERT(DockContextFindNodeByID(ctx, id) == NULL);

        // We don't set node->LastFrameAlive on construction. Nodes are always created at all time to reflect .ini settings!
        IMGUI_DEBUG_LOG_DOCKING("[docking] DockContextAddNode 0x%08X\n", id);
        ImGuiDockNode* node = IM_NEW(ImGuiDockNode)(id);
        ctx->DockContext.Nodes.SetVoidPtr(node->ID, node);
        return node;
    }

    static void DockNodeSetupHostWindow(ImGuiDockNode* node, ImGuiWindow* host_window)
    {
        // Remove ourselves from any previous different host window
        // This can happen if a user mistakenly does (see #4295 for details):
        //  - N+0: DockBuilderAddNode(id, 0)    // missing ImGuiDockNodeFlags_DockSpace
        //  - N+1: NewFrame()                   // will create floating host window for that node
        //  - N+1: DockSpace(id)                // requalify node as dockspace, moving host window
        if (node->HostWindow && node->HostWindow != host_window && node->HostWindow->DockNodeAsHost == node)
            node->HostWindow->DockNodeAsHost = NULL;

        host_window->DockNodeAsHost = node;
        node->HostWindow = host_window;
    }

    static void DockNodeUpdateForRootNode(ImGuiDockNode* node)
    {
        DockNodeUpdateFlagsAndCollapse(node);

        // - Setup central node pointers
        // - Find if there's only a single visible window in the hierarchy (in which case we need to display a regular title bar -> FIXME-DOCK: that last part is not done yet!)
        // Cannot merge this with DockNodeUpdateFlagsAndCollapse() because FirstNodeWithWindows is found after window removal and child collapsing
        ImGuiDockNodeTreeInfo info;
        DockNodeFindInfo(node, &info);
        node->CentralNode = info.CentralNode;
        node->OnlyNodeWithWindows = (info.CountNodesWithWindows == 1) ? info.FirstNodeWithWindows : NULL;
        node->CountNodeWithWindows = info.CountNodesWithWindows;
        if (node->LastFocusedNodeId == 0 && info.FirstNodeWithWindows != NULL)
            node->LastFocusedNodeId = info.FirstNodeWithWindows->ID;

        // Copy the window class from of our first window so it can be used for proper dock filtering.
        // When node has mixed windows, prioritize the class with the most constraint (DockingAllowUnclassed = false) as the reference to copy.
        // FIXME-DOCK: We don't recurse properly, this code could be reworked to work from DockNodeUpdateScanRec.
        if (ImGuiDockNode* first_node_with_windows = info.FirstNodeWithWindows)
        {
            node->WindowClass = first_node_with_windows->Windows[0]->WindowClass;
            for (int n = 1; n < first_node_with_windows->Windows.Size; n++)
                if (first_node_with_windows->Windows[n]->WindowClass.DockingAllowUnclassed == false)
                {
                    node->WindowClass = first_node_with_windows->Windows[n]->WindowClass;
                    break;
                }
        }

        ImGuiDockNode* mark_node = node->CentralNode;
        while (mark_node)
        {
            mark_node->HasCentralNodeChild = true;
            mark_node = mark_node->ParentNode;
        }
    }










    static void DockNodeUpdate(ImGuiDockNode* node)
    {
        ImGuiContext& g = *GImGui;
        IM_ASSERT(node->LastFrameActive != g.FrameCount);
        node->LastFrameAlive = g.FrameCount;
        node->IsBgDrawnThisFrame = false;

        node->CentralNode = node->OnlyNodeWithWindows = NULL;
        if (node->IsRootNode())
            DockNodeUpdateForRootNode(node);

        // Remove tab bar if not needed
        if (node->TabBar && node->IsNoTabBar())
            DockNodeRemoveTabBar(node);

        // Early out for hidden root dock nodes (when all DockId references are in inactive windows, or there is only 1 floating window holding on the DockId)
        bool want_to_hide_host_window = false;
        if (node->IsFloatingNode())
        {
            if (node->Windows.Size <= 1 && node->IsLeafNode())
                if (!g.IO.ConfigDockingAlwaysTabBar && (node->Windows.Size == 0 || !node->Windows[0]->WindowClass.DockingAlwaysTabBar))
                    want_to_hide_host_window = true;
            if (node->CountNodeWithWindows == 0)
                want_to_hide_host_window = true;
        }
        if (want_to_hide_host_window)
        {
            if (node->Windows.Size == 1)
            {
                // Floating window pos/size is authoritative
                ImGuiWindow* single_window = node->Windows[0];
                node->Pos = single_window->Pos;
                node->Size = single_window->SizeFull;
                node->AuthorityForPos = node->AuthorityForSize = node->AuthorityForViewport = ImGuiDataAuthority_Window;

                // Transfer focus immediately so when we revert to a regular window it is immediately selected
                if (node->HostWindow && g.NavWindow == node->HostWindow)
                    FocusWindow(single_window);
                if (node->HostWindow)
                {
                    single_window->Viewport = node->HostWindow->Viewport;
                    single_window->ViewportId = node->HostWindow->ViewportId;
                    if (node->HostWindow->ViewportOwned)
                    {
                        single_window->Viewport->Window = single_window;
                        single_window->ViewportOwned = true;
                    }
                }
            }

            DockNodeHideHostWindow(node);
            node->State = ImGuiDockNodeState_HostWindowHiddenBecauseSingleWindow;
            node->WantCloseAll = false;
            node->WantCloseTabId = 0;
            node->HasCloseButton = node->HasWindowMenuButton = false;
            node->LastFrameActive = g.FrameCount;

            if (node->WantMouseMove && node->Windows.Size == 1)
                DockNodeStartMouseMovingWindow(node, node->Windows[0]);
            return;
        }

        // In some circumstance we will defer creating the host window (so everything will be kept hidden),
        // while the expected visible window is resizing itself.
        // This is important for first-time (no ini settings restored) single window when io.ConfigDockingAlwaysTabBar is enabled,
        // otherwise the node ends up using the minimum window size. Effectively those windows will take an extra frame to show up:
        //   N+0: Begin(): window created (with no known size), node is created
        //   N+1: DockNodeUpdate(): node skip creating host window / Begin(): window size applied, not visible
        //   N+2: DockNodeUpdate(): node can create host window / Begin(): window becomes visible
        // We could remove this frame if we could reliably calculate the expected window size during node update, before the Begin() code.
        // It would require a generalization of CalcWindowExpectedSize(), probably extracting code away from Begin().
        // In reality it isn't very important as user quickly ends up with size data in .ini file.
        if (node->IsVisible && node->HostWindow == NULL && node->IsFloatingNode() && node->IsLeafNode())
        {
            IM_ASSERT(node->Windows.Size > 0);
            ImGuiWindow* ref_window = NULL;
            if (node->SelectedTabId != 0) // Note that we prune single-window-node settings on .ini loading, so this is generally 0 for them!
                ref_window = DockNodeFindWindowByID(node, node->SelectedTabId);
            if (ref_window == NULL)
                ref_window = node->Windows[0];
            if (ref_window->AutoFitFramesX > 0 || ref_window->AutoFitFramesY > 0)
            {
                node->State = ImGuiDockNodeState_HostWindowHiddenBecauseWindowsAreResizing;
                return;
            }
        }

        const ImGuiDockNodeFlags node_flags = node->MergedFlags;

        // Decide if the node will have a close button and a window menu button
        node->HasWindowMenuButton = (node->Windows.Size > 0) && (node_flags & ImGuiDockNodeFlags_NoWindowMenuButton) == 0;
        node->HasCloseButton = false;
        for (int window_n = 0; window_n < node->Windows.Size; window_n++)
        {
            // FIXME-DOCK: Setting DockIsActive here means that for single active window in a leaf node, DockIsActive will be cleared until the next Begin() call.
            ImGuiWindow* window = node->Windows[window_n];
            node->HasCloseButton |= window->HasCloseButton;
            window->DockIsActive = (node->Windows.Size > 1);
        }
        if (node_flags & ImGuiDockNodeFlags_NoCloseButton)
            node->HasCloseButton = false;

        // Bind or create host window
        ImGuiWindow* host_window = NULL;
        bool beginned_into_host_window = false;
        if (node->IsDockSpace())
        {
            // [Explicit root dockspace node]
            IM_ASSERT(node->HostWindow);
            host_window = node->HostWindow;
        }
        else
        {
            // [Automatic root or child nodes]
            if (node->IsRootNode() && node->IsVisible)
            {
                ImGuiWindow* ref_window = (node->Windows.Size > 0) ? node->Windows[0] : NULL;

                // Sync Pos
                if (node->AuthorityForPos == ImGuiDataAuthority_Window && ref_window)
                    SetNextWindowPos(ref_window->Pos);
                else if (node->AuthorityForPos == ImGuiDataAuthority_DockNode)
                    SetNextWindowPos(node->Pos);

                // Sync Size
                if (node->AuthorityForSize == ImGuiDataAuthority_Window && ref_window)
                    SetNextWindowSize(ref_window->SizeFull);
                else if (node->AuthorityForSize == ImGuiDataAuthority_DockNode)
                    SetNextWindowSize(node->Size);

                // Sync Collapsed
                if (node->AuthorityForSize == ImGuiDataAuthority_Window && ref_window)
                    SetNextWindowCollapsed(ref_window->Collapsed);

                // Sync Viewport
                if (node->AuthorityForViewport == ImGuiDataAuthority_Window && ref_window)
                    SetNextWindowViewport(ref_window->ViewportId);

                SetNextWindowClass(&node->WindowClass);

                // Begin into the host window
                char window_label[20];
                DockNodeGetHostWindowTitle(node, window_label, IM_ARRAYSIZE(window_label));
                ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_DockNodeHost;
                window_flags |= ImGuiWindowFlags_NoFocusOnAppearing;
                window_flags |= ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoCollapse;
                window_flags |= ImGuiWindowFlags_NoTitleBar;

                SetNextWindowBgAlpha(0.0f); // Don't set ImGuiWindowFlags_NoBackground because it disables borders
                PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                Begin(window_label, NULL, window_flags);
                PopStyleVar();
                beginned_into_host_window = true;

                host_window = g.CurrentWindow;
                DockNodeSetupHostWindow(node, host_window);
                host_window->DC.CursorPos = host_window->Pos;
                node->Pos = host_window->Pos;
                node->Size = host_window->Size;

                // We set ImGuiWindowFlags_NoFocusOnAppearing because we don't want the host window to take full focus (e.g. steal NavWindow)
                // But we still it bring it to the front of display. There's no way to choose this precise behavior via window flags.
                // One simple case to ponder if: window A has a toggle to create windows B/C/D. Dock B/C/D together, clear the toggle and enable it again.
                // When reappearing B/C/D will request focus and be moved to the top of the display pile, but they are not linked to the dock host window
                // during the frame they appear. The dock host window would keep its old display order, and the sorting in EndFrame would move B/C/D back
                // after the dock host window, losing their top-most status.
                if (node->HostWindow->Appearing)
                    BringWindowToDisplayFront(node->HostWindow);

                node->AuthorityForPos = node->AuthorityForSize = node->AuthorityForViewport = ImGuiDataAuthority_Auto;
            }
            else if (node->ParentNode)
            {
                node->HostWindow = host_window = node->ParentNode->HostWindow;
                node->AuthorityForPos = node->AuthorityForSize = node->AuthorityForViewport = ImGuiDataAuthority_Auto;
            }
            if (node->WantMouseMove && node->HostWindow)
                DockNodeStartMouseMovingWindow(node, node->HostWindow);
        }

        // Update focused node (the one whose title bar is highlight) within a node tree
        if (node->IsSplitNode())
            IM_ASSERT(node->TabBar == NULL);
        if (node->IsRootNode())
            if (g.NavWindow && g.NavWindow->RootWindow->DockNode && g.NavWindow->RootWindow->ParentWindow == host_window)
                node->LastFocusedNodeId = g.NavWindow->RootWindow->DockNode->ID;

        // Register a hit-test hole in the window unless we are currently dragging a window that is compatible with our dockspace
        ImGuiDockNode* central_node = node->CentralNode;
        const bool central_node_hole = node->IsRootNode() && host_window && (node_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0 && central_node != NULL && central_node->IsEmpty();
        bool central_node_hole_register_hit_test_hole = central_node_hole;
        if (central_node_hole)
            if (const ImGuiPayload* payload = ImGui::GetDragDropPayload())
                if (payload->IsDataType(IMGUI_PAYLOAD_TYPE_WINDOW) && DockNodeIsDropAllowed(host_window, *(ImGuiWindow**)payload->Data))
                    central_node_hole_register_hit_test_hole = false;
        if (central_node_hole_register_hit_test_hole)
        {
            // We add a little padding to match the "resize from edges" behavior and allow grabbing the splitter easily.
            // (But we only add it if there's something else on the other side of the hole, otherwise for e.g. fullscreen
            // covering passthru node we'd have a gap on the edge not covered by the hole)
            IM_ASSERT(node->IsDockSpace()); // We cannot pass this flag without the DockSpace() api. Testing this because we also setup the hole in host_window->ParentNode
            ImGuiDockNode* root_node = DockNodeGetRootNode(central_node);
            ImRect root_rect(root_node->Pos, root_node->Pos + root_node->Size);
            ImRect hole_rect(central_node->Pos, central_node->Pos + central_node->Size);
            if (hole_rect.Min.x > root_rect.Min.x) { hole_rect.Min.x += WINDOWS_HOVER_PADDING; }
            if (hole_rect.Max.x < root_rect.Max.x) { hole_rect.Max.x -= WINDOWS_HOVER_PADDING; }
            if (hole_rect.Min.y > root_rect.Min.y) { hole_rect.Min.y += WINDOWS_HOVER_PADDING; }
            if (hole_rect.Max.y < root_rect.Max.y) { hole_rect.Max.y -= WINDOWS_HOVER_PADDING; }
            //GetForegroundDrawList()->AddRect(hole_rect.Min, hole_rect.Max, IM_COL32(255, 0, 0, 255));
            if (central_node_hole && !hole_rect.IsInverted())
            {
                SetWindowHitTestHole(host_window, hole_rect.Min, hole_rect.Max - hole_rect.Min);
                if (host_window->ParentWindow)
                    SetWindowHitTestHole(host_window->ParentWindow, hole_rect.Min, hole_rect.Max - hole_rect.Min);
            }
        }

        // Update position/size, process and draw resizing splitters
        if (node->IsRootNode() && host_window)
        {
            DockNodeTreeUpdatePosSize(node, host_window->Pos, host_window->Size);
            DockNodeTreeUpdateSplitter(node);
        }

        // Draw empty node background (currently can only be the Central Node)
        if (host_window && node->IsEmpty() && node->IsVisible)
        {
            host_window->DrawList->ChannelsSetCurrent(DOCKING_HOST_DRAW_CHANNEL_BG);
            node->LastBgColor = (node_flags & ImGuiDockNodeFlags_PassthruCentralNode) ? 0 : GetColorU32(ImGuiCol_DockingEmptyBg);
            if (node->LastBgColor != 0)
                host_window->DrawList->AddRectFilled(node->Pos, node->Pos + node->Size, node->LastBgColor);
            node->IsBgDrawnThisFrame = true;
        }

        // Draw whole dockspace background if ImGuiDockNodeFlags_PassthruCentralNode if set.
        // We need to draw a background at the root level if requested by ImGuiDockNodeFlags_PassthruCentralNode, but we will only know the correct pos/size
        // _after_ processing the resizing splitters. So we are using the DrawList channel splitting facility to submit drawing primitives out of order!
        const bool render_dockspace_bg = node->IsRootNode() && host_window && (node_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0;
        if (render_dockspace_bg && node->IsVisible)
        {
            host_window->DrawList->ChannelsSetCurrent(DOCKING_HOST_DRAW_CHANNEL_BG);
            if (central_node_hole)
                RenderRectFilledWithHole(host_window->DrawList, node->Rect(), central_node->Rect(), GetColorU32(ImGuiCol_WindowBg), 0.0f);
            else
                host_window->DrawList->AddRectFilled(node->Pos, node->Pos + node->Size, GetColorU32(ImGuiCol_WindowBg), 0.0f);
        }

        // Draw and populate Tab Bar
        if (host_window)
            host_window->DrawList->ChannelsSetCurrent(DOCKING_HOST_DRAW_CHANNEL_FG);
        if (host_window && node->Windows.Size > 0)
        {
            DockNodeUpdateTabBar(node, host_window);
        }
        else
        {
            node->WantCloseAll = false;
            node->WantCloseTabId = 0;
            node->IsFocused = false;
        }
        if (node->TabBar && node->TabBar->SelectedTabId)
            node->SelectedTabId = node->TabBar->SelectedTabId;
        else if (node->Windows.Size > 0)
            node->SelectedTabId = node->Windows[0]->TabId;

        // Draw payload drop target
        if (host_window && node->IsVisible)
            if (node->IsRootNode() && (g.MovingWindow == NULL || g.MovingWindow->RootWindowDockTree != host_window))
                BeginDockableDragDropTarget(host_window);

        // We update this after DockNodeUpdateTabBar()
        node->LastFrameActive = g.FrameCount;

        // Recurse into children
        // FIXME-DOCK FIXME-OPT: Should not need to recurse into children
        if (host_window)
        {
            if (node->ChildNodes[0])
                DockNodeUpdate(node->ChildNodes[0]);
            if (node->ChildNodes[1])
                DockNodeUpdate(node->ChildNodes[1]);

            // Render outer borders last (after the tab bar)
            if (node->IsRootNode())
                RenderWindowOuterBorders(host_window);
        }

        // End host window
        if (beginned_into_host_window) //-V1020
            End();
    }
}




namespace Aquarius
{
    using namespace ImGui;


    ImGuiID DockSpace(ImGuiID id, const ImVec2& size_arg, ImGuiDockNodeFlags flags, const ImGuiWindowClass* window_class)
    {
        ImGuiContext* ctx = GImGui;
        ImGuiContext& g = *ctx;
        ImGuiWindow* window = GetCurrentWindow();
        if (!(g.IO.ConfigFlags & ImGuiConfigFlags_DockingEnable))
            return 0;

        // Early out if parent window is hidden/collapsed
        // This is faster but also DockNodeUpdateTabBar() relies on TabBarLayout() running (which won't if SkipItems=true) to set NextSelectedTabId = 0). See #2960.
        // If for whichever reason this is causing problem we would need to ensure that DockNodeUpdateTabBar() ends up clearing NextSelectedTabId even if SkipItems=true.
        if (window->SkipItems)
            flags |= ImGuiDockNodeFlags_KeepAliveOnly;

        IM_ASSERT((flags & ImGuiDockNodeFlags_DockSpace) == 0);
        IM_ASSERT(id != 0);
        ImGuiDockNode* node = DockContextFindNodeByID(ctx, id);
        if (!node)
        {
            IMGUI_DEBUG_LOG_DOCKING("[docking] DockSpace: dockspace node 0x%08X created\n", id);
            node = DockContextAddNode(ctx, id);
            node->SetLocalFlags(ImGuiDockNodeFlags_CentralNode);
        }
        if (window_class && window_class->ClassId != node->WindowClass.ClassId)
            IMGUI_DEBUG_LOG_DOCKING("[docking] DockSpace: dockspace node 0x%08X: setup WindowClass 0x%08X -> 0x%08X\n", id, node->WindowClass.ClassId, window_class->ClassId);
        node->SharedFlags = flags;
        node->WindowClass = window_class ? *window_class : ImGuiWindowClass();

        // When a DockSpace transitioned form implicit to explicit this may be called a second time
        // It is possible that the node has already been claimed by a docked window which appeared before the DockSpace() node, so we overwrite IsDockSpace again.
        if (node->LastFrameActive == g.FrameCount && !(flags & ImGuiDockNodeFlags_KeepAliveOnly))
        {
            IM_ASSERT(node->IsDockSpace() == false && "Cannot call DockSpace() twice a frame with the same ID");
            node->SetLocalFlags(node->LocalFlags | ImGuiDockNodeFlags_DockSpace);
            return id;
        }
        node->SetLocalFlags(node->LocalFlags | ImGuiDockNodeFlags_DockSpace);

        // Keep alive mode, this is allow windows docked into this node so stay docked even if they are not visible
        if (flags & ImGuiDockNodeFlags_KeepAliveOnly)
        {
            node->LastFrameAlive = g.FrameCount;
            return id;
        }

        const ImVec2 content_avail = GetContentRegionAvail();
        ImVec2 size = ImFloor(size_arg);
        if (size.x <= 0.0f)
            size.x = ImMax(content_avail.x + size.x, 4.0f); // Arbitrary minimum child size (0.0f causing too much issues)
        if (size.y <= 0.0f)
            size.y = ImMax(content_avail.y + size.y, 4.0f);
        IM_ASSERT(size.x > 0.0f && size.y > 0.0f);

        node->Pos = window->DC.CursorPos;
        node->Size = node->SizeRef = size;
        SetNextWindowPos(node->Pos);
        SetNextWindowSize(node->Size);
        g.NextWindowData.PosUndock = false;

        // FIXME-DOCK: Why do we need a child window to host a dockspace, could we host it in the existing window?
        // FIXME-DOCK: What is the reason for not simply calling BeginChild()? (OK to have a reason but should be commented)
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_DockNodeHost;
        window_flags |= ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
        window_flags |= ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        window_flags |= ImGuiWindowFlags_NoBackground;

        char title[256];
        ImFormatString(title, IM_ARRAYSIZE(title), "%s/DockSpace_%08X", window->Name, id);

        PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
        Begin(title, NULL, window_flags);
        PopStyleVar();

        ImGuiWindow* host_window = g.CurrentWindow;
        DockNodeSetupHostWindow(node, host_window);
        host_window->ChildId = window->GetID(title);
        node->OnlyNodeWithWindows = NULL;

        IM_ASSERT(node->IsRootNode());

        // We need to handle the rare case were a central node is missing.
        // This can happen if the node was first created manually with DockBuilderAddNode() but _without_ the ImGuiDockNodeFlags_Dockspace.
        // Doing it correctly would set the _CentralNode flags, which would then propagate according to subsequent split.
        // It would also be ambiguous to attempt to assign a central node while there are split nodes, so we wait until there's a single node remaining.
        // The specific sub-property of _CentralNode we are interested in recovering here is the "Don't delete when empty" property,
        // as it doesn't make sense for an empty dockspace to not have this property.
        if (node->IsLeafNode() && !node->IsCentralNode())
            node->SetLocalFlags(node->LocalFlags | ImGuiDockNodeFlags_CentralNode);

        // Update the node
        DockNodeUpdate(node);

        End();
        ItemSize(size);
        return id;
    }
}
