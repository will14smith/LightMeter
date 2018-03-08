import { Window, Action, WindowManagerInterface, WindowType } from "../types";
import sensor from "../sensor";

export default class MeterWindow implements Window {
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
    if (Date.now() - sensor.lastRead < 100) {
      return;
    }

    const ctx = this.ctx;
    ctx.fillStyle = "black";
    ctx.fillRect(
      this.wm.windowBounds.x,
      this.wm.windowBounds.y,
      this.wm.windowBounds.w,
      this.wm.windowBounds.h
    );

    this.renderWindow();
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
    ctx.fillText("Meter", 2, 1);

    const width = ctx.measureText("Meter").width;

    // line to right of text
    ctx.fillRect(width + 4, 0, 1, 10);

    ctx.restore();
  }

  renderWindow(): void {
    const ctx = this.ctx;

    ctx.save();
    ctx.translate(this.wm.windowBounds.x, this.wm.windowBounds.y);

    ctx.fillStyle = "white";
    ctx.font = "20px monospace";
    ctx.textAlign = "left";
    ctx.textBaseline = "top";

    ctx.fillText(`${Math.round(sensor.lux)} lx`, 0, 0);
    ctx.fillText(`${Math.round(sensor.temperature)} K`, 0, 20);

    ctx.restore();
  }
}
