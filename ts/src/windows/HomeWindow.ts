import { Window, Action, WindowManagerInterface, WindowType } from "../types";

export default class HomeWindow implements Window {
  private static items = [
    { text: "Exposure", windowType: WindowType.Exposure },
    { text: "Aperture", windowType: WindowType.Aperture },
    { text: "ISO", windowType: WindowType.ISO },
    { text: "Meter", windowType: WindowType.Meter },
    { text: "Settings", windowType: WindowType.Settings }
  ];

  private wm: WindowManagerInterface;
  private ctx: CanvasRenderingContext2D;

  private index: number;
  private flash: boolean;
  private lastFlash: number;

  activate(newwm: WindowManagerInterface, ctx: CanvasRenderingContext2D): void {
    this.wm = newwm;
    this.ctx = ctx;
    this.index = 0;
  }
  deactive(): void {
    // TODO
  }

  handle(action: Action): void {
    if (action === Action.Down) {
      this.setIndex((this.index + 1) % 5);
    } else if (action === Action.Up) {
      this.setIndex((this.index + 4) % 5);
    } else if (action === Action.Press || action === Action.LongPress) {
      const newWindow = HomeWindow.items[this.index].windowType;
      this.wm.setWindow(newWindow);
    }
  }

  loop() {
    if (!this.lastFlash) {
      this.lastFlash = Date.now();
      return;
    }

    if (Date.now() - this.lastFlash < 500) {
      return;
    }

    this.lastFlash = Date.now();
    this.flash = !this.flash;

    const ctx = this.ctx;
    ctx.save();
    ctx.translate(this.wm.windowBounds.x, this.wm.windowBounds.y);
    this.renderItem(this.index, true);
    ctx.restore();
  }

  renderStatusBar(): void {
    const ctx = this.ctx;

    ctx.save();
    ctx.translate(this.wm.statusBarBounds.x, this.wm.statusBarBounds.y);

    // status text
    ctx.fillStyle = "white";
    ctx.font = "8px monospace";
    ctx.textAlign = "left";
    ctx.textBaseline = "top";
    ctx.fillText("Menu", 2, 1);

    const width = ctx.measureText("Menu").width;

    // line to right of text
    ctx.fillRect(width + 4, 0, 1, 10);

    ctx.restore();
  }

  renderWindow(): void {
    const ctx = this.ctx;

    ctx.save();
    ctx.translate(this.wm.windowBounds.x, this.wm.windowBounds.y);

    for (var i = 0; i < HomeWindow.items.length; i++) {
      this.renderItem(i, false);
    }

    ctx.restore();
  }

  private setIndex(index: number) {
    this.lastFlash = Date.now();
    this.flash = false;

    const prevIndex = this.index;
    this.index = index;

    const ctx = this.ctx;

    ctx.save();
    ctx.translate(this.wm.windowBounds.x, this.wm.windowBounds.y);

    this.renderItem(prevIndex, true);
    this.renderItem(index, true);

    ctx.restore();
  }

  private renderItem(index: number, full: boolean = false) {
    const item = HomeWindow.items[index];

    const text = `${
      this.index === index ? (this.flash ? "  " : "> ") : index + 1 + "."
    } ${item.text}`;

    const ctx = this.ctx;

    if (full) {
      ctx.fillStyle = "black";
      ctx.fillRect(0, index * 10, this.wm.windowBounds.w, 10);
    }

    ctx.fillStyle = "white";
    ctx.font = "10px monospace";
    ctx.textAlign = "left";
    ctx.textBaseline = "middle";

    ctx.fillText(text, 2, index * 10 + 5);
  }
}
