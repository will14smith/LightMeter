import WindowManager from "./WindowManager";
import HomeWindow from "./windows/HomeWindow";

const root = document.getElementById("root") as HTMLElement;
const wm = new WindowManager(createCanvas(), new HomeWindow());

createButton("Up", () => wm.up());
createButton("Down", () => wm.down());
createButton("Press", () => wm.press());

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