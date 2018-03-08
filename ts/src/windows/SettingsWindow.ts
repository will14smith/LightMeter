import { Window, Action, WindowManagerInterface, WindowType } from "../types";

export default class SettingsWindow implements Window {
  private wm: WindowManagerInterface;
  private ctx: CanvasRenderingContext2D;

  activate(newwm: WindowManagerInterface, ctx: CanvasRenderingContext2D): void {
    this.wm = newwm;
    this.ctx = ctx;
  }
  deactive(): void {
    // TODO
  }

  handle(action: Action): void {
    if (action === Action.LongPress) {
      this.wm.setWindow(WindowType.Home);
    }
  }

  loop() {
    // no-op
  }

  renderStatusBar(): void {
    const ctx = this.ctx;

    ctx.save();
    ctx.translate(this.wm.statusBarBounds.x, this.wm.statusBarBounds.y);

    // status text
    ctx.fillStyle = "white";
    ctx.font = "8px sans-serif";
    ctx.textAlign = "left";
    ctx.textBaseline = "top";
    ctx.fillText("Settings", 2, 1);

    const width = ctx.measureText("Settings").width;

    // line to right of text
    ctx.fillRect(width + 4, 0, 1, 10);

    ctx.restore();
  }

  renderWindow(): void {
    const ctx = this.ctx;

    ctx.save();
    ctx.translate(this.wm.windowBounds.x, this.wm.windowBounds.y);

    ctx.restore();
  }
}
