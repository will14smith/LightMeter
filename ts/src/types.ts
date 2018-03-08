export enum Action {
  Up,
  Down,
  Press
}

export interface BoundingBox {
  x: number;
  y: number;
  w: number;
  h: number;
}

export interface Window {
  activate(wm: WindowManagerInterface, ctx: CanvasRenderingContext2D): void;
  deactive(): void;

  handle(action: Action): void;
  loop(): void;

  renderStatusBar(): void;
  renderWindow(): void;
}

export enum WindowType {
  Home,
  Exposure,
  Aperture,
  ISO,
  Meter,
  Settings
}

export interface WindowManagerInterface {
  statusBarBounds: BoundingBox;
  windowBounds: BoundingBox;

  setWindow(name: WindowType): void;
}
