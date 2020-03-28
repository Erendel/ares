struct ManifestViewer : VerticalLayout {
  auto construct() -> void;
  auto reload() -> void;
  auto unload() -> void;

  Label manifestLabel{this, Size{~0, 0}, 2};
  TextEdit manifestView{this, Size{~0, ~0}};
};

struct MemoryEditor : VerticalLayout {
  auto construct() -> void;
  auto reload() -> void;
  auto unload() -> void;
  auto refresh() -> void;
  auto liveRefresh() -> void;
  auto eventChange() -> void;
  auto eventExport() -> void;
  auto setVisible(bool visible = true) -> MemoryEditor&;

  Label memoryLabel{this, Size{~0, 0}, 2};
  ComboButton memoryList{this, Size{~0, 0}};
  HexEdit memoryEditor{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Button exportButton{&controlLayout, Size{80, 0}};
    Label gotoLabel{&controlLayout, Size{0, 0}, 2};
    LineEdit gotoAddress{&controlLayout, Size{70, 0}};
    Widget spacer{&controlLayout, Size{~0, 0}};
    CheckLabel liveOption{&controlLayout, Size{0, 0}, 2};
    Button refreshButton{&controlLayout, Size{80, 0}};
};

struct GraphicsViewer : VerticalLayout {
  auto construct() -> void;
  auto reload() -> void;
  auto unload() -> void;
  auto refresh() -> void;
  auto liveRefresh() -> void;
  auto eventChange() -> void;
  auto eventExport() -> void;
  auto setVisible(bool visible = true) -> GraphicsViewer&;

  Label graphicsLabel{this, Size{~0, 0}, 2};
  ComboButton graphicsList{this, Size{~0, 0}};
  Canvas graphicsView{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Button exportButton{&controlLayout, Size{80, 0}};
    Widget spacer{&controlLayout, Size{~0, 0}};
    CheckLabel liveOption{&controlLayout, Size{0, 0}, 2};
    Button refreshButton{&controlLayout, Size{80, 0}};
};

struct TraceLogger : VerticalLayout {
  auto construct() -> void;
  auto reload() -> void;
  auto unload() -> void;
  auto eventToggle(ListViewItem) -> void;

  file_buffer fp;

  Label tracerLabel{this, Size{~0, 0}, 2};
  ListView tracerList{this, Size{~0, ~0}};
};

struct ToolsWindow : Window {
  ToolsWindow();
  auto show(const string& panel) -> void;
  auto eventChange() -> void;

  HorizontalLayout layout{this};
    ListView panelList{&layout, Size{125_sx, ~0}};
    VerticalLayout panelContainer{&layout, Size{~0, ~0}};
      ManifestViewer manifestViewer;
      MemoryEditor memoryEditor;
      GraphicsViewer graphicsViewer;
      TraceLogger traceLogger;
      HomePanel homePanel;
};

namespace Instances { extern Instance<ToolsWindow> toolsWindow; }
extern ToolsWindow& toolsWindow;
extern ManifestViewer& manifestViewer;
extern MemoryEditor& memoryEditor;
extern GraphicsViewer& graphicsViewer;
extern TraceLogger& traceLogger;