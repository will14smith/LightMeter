import WindowManager, { Window, Action, BoundingBox } from "../WindowManager";

// TODO create actual windows

export default class HomeWindow implements Window {
  private static items = [
    { text: "Exposure", windowFactory: () => new HomeWindow() },
    { text: "Aparture", windowFactory: () => new HomeWindow() },
    { text: "ISO", windowFactory: () => new HomeWindow() },
    { text: "Meter", windowFactory: () => new HomeWindow() },
    { text: "Settings", windowFactory: () => new HomeWindow() }
  ];

  private wm: WindowManager;
  private index: number;

  activate(newwm: WindowManager): void {
    this.wm = newwm;
    this.index = 0;
  }
  deactive(): void {
    // TODO
  }

  handle(action: Action): void {
    if (action === Action.down) {
      this.index = (this.index + 1) % 5;
    }
    if (action === Action.up) {
      this.index = (this.index + 4) % 5;
    }
    if (action === Action.press) {
      const newWindow = HomeWindow.items[this.index].windowFactory();
      this.wm.setWindow(newWindow);
    }

    this.wm.render();
  }

  renderStatusBar(ctx: CanvasRenderingContext2D, bounds: BoundingBox): void {
    ctx.save();
    ctx.translate(bounds.x, bounds.y);

    // status text
    ctx.fillStyle = "white";
    ctx.font = "8px sans-serif";
    ctx.textAlign = "left";
    ctx.textBaseline = "top";
    ctx.fillText("Menu", 2, 1);

    const width = ctx.measureText("Menu").width;

    // line to right of text
    ctx.fillRect(width + 4, 0, 1, 10);

    ctx.restore();
  }

  renderWindow(ctx: CanvasRenderingContext2D, bounds: BoundingBox): void {
    ctx.save();
    ctx.translate(bounds.x, bounds.y);

    ctx.fillStyle = "white";
    ctx.font = "10px sans-serif";
    ctx.textAlign = "left";
    ctx.textBaseline = "top";

    for (var i = 0; i < HomeWindow.items.length; i++) {
      const item = HomeWindow.items[i];

      const text = `${this.index === i ? "> " : i + 1 + "."} ${item.text}`;

      ctx.fillText(text, 2, i * 10);
    }

    ctx.restore();
  }
}
