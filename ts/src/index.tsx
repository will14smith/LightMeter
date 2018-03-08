import WindowManager from "./WindowManager";
import { Action } from "./types";

const root = document.getElementById("root") as HTMLElement;
const wm = new WindowManager(createCanvas());

root.appendChild(document.createElement("br"));

createButton("Up", () => wm.handle(Action.Up));
createButton("Down", () => wm.handle(Action.Down));
createButton("Press", () => wm.handle(Action.Press));
createButton("LongPress", () => wm.handle(Action.LongPress));

requestAnimationFrame(loop);
function loop() {
  wm.loop();
  requestAnimationFrame(loop);
}

function createCanvas() {
  const canvas = document.createElement("canvas");

  root.appendChild(canvas);

  const ctx = canvas.getContext("2d");
  if (ctx === null) {
    throw new Error("AHHH");
  }

  const scale = 8;
  canvas.width = 128 * scale;
  canvas.height = 64 * scale;

  ctx.scale(scale, scale);

  return ctx;
}

function createButton(text: string, onClick: () => void) {
  const button = document.createElement("button");

  button.innerText = text;
  button.addEventListener("click", onClick, false);

  root.appendChild(button);

  return button;
}
