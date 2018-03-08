import {
  Window,
  Action,
  WindowManagerInterface,
  WindowType,
  BoundingBox
} from "./types";

import HomeWindow from "./windows/HomeWindow";
import ExposureWindow from "./windows/ExposureWindow";
import ApertureWindow from "./windows/ApertureWindow";
import ISOWindow from "./windows/ISOWindow";
import MeterWindow from "./windows/MeterWindow";
import SettingsWindow from "./windows/SettingsWindow";

function windowFactory(type: WindowType): Window {
  switch (type) {
    case WindowType.Home:
      return new HomeWindow();
    case WindowType.Exposure:
      return new ExposureWindow();
    case WindowType.Aperture:
      return new ApertureWindow();
    case WindowType.ISO:
      return new ISOWindow();
    case WindowType.Meter:
      return new MeterWindow();
    case WindowType.Settings:
      return new SettingsWindow();

    default:
      throw new Error("Unknown window type");
  }
}

export default class WindowManager implements WindowManagerInterface {
  public statusBarBounds: BoundingBox = { x: 0, y: 0, w: 105, h: 10 };
  public windowBounds: BoundingBox = { x: 0, y: 11, w: 128, h: 54 };

  private ctx: CanvasRenderingContext2D;
  private window: Window;

  constructor(ctx: CanvasRenderingContext2D) {
    this.ctx = ctx;
    this.setWindow(WindowType.Home);
  }

  handle(action: Action) {
    this.window.handle(action);
  }

  loop() {
    this.window.loop();
  }

  setWindow(windowType: WindowType) {
    if (this.window) {
      this.window.deactive();
    }

    this.window = windowFactory(windowType);
    this.window.activate(this, this.ctx);

    this.render(true);
  }

  render(full: boolean = true) {
    if (full) {
      this.clearScreen();
      this.renderGlobalStatusBar();
    }

    this.window.renderStatusBar();
    this.window.renderWindow();
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
