export enum Action {
  up,
  down,
  press
}

export interface BoundingBox {
  x: number;
  y: number;
  w: number;
  h: number;
}

export interface Window {
  activate(wm: WindowManager): void;
  deactive(): void;

  handle(action: Action): void;

  renderStatusBar(ctx: CanvasRenderingContext2D, bounds: BoundingBox): void;
  renderWindow(ctx: CanvasRenderingContext2D, bounds: BoundingBox): void;
}

export default class WindowManager {
  private ctx: CanvasRenderingContext2D;
  private window: Window;

  constructor(ctx: CanvasRenderingContext2D, window: Window) {
    this.ctx = ctx;
    this.window = window;
    this.window.activate(this);
    this.render();
  }

  up() {
    this.window.handle(Action.up);
  }
  down() {
    this.window.handle(Action.down);
  }
  press() {
    this.window.handle(Action.press);
  }

  setWindow(window: Window) {
    this.window.deactive();
    this.window = window;
    this.window.activate(this);
  }

  render(full: boolean = true) {
    if (full) {
      this.clearScreen();
      this.renderGlobalStatusBar();
    }

    const windowStatusBoundingBox = { x: 0, y: 0, w: 105, h: 10 };
    const windowBoundingBox = { x: 0, y: 11, w: 128, h: 54 };

    this.window.renderStatusBar(this.ctx, windowStatusBoundingBox);
    this.window.renderWindow(this.ctx, windowBoundingBox);
  }

  private clearScreen() {
    this.ctx.fillStyle = "black";
    this.ctx.fillRect(0, 0, 128, 64);
  }

  private renderGlobalStatusBar() {
    const ctx = this.ctx;

    ctx.fillStyle = "white";

    // line under bar
    ctx.fillRect(0, 10, 128, 1);

    // battery icon
    ctx.fillRect(109, 1, 18, 8);

    // line to left of global
    ctx.fillRect(107, 0, 1, 10);
  }
}
