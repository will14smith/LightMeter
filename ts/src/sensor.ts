class Sensor {
  public lux: number;
  public temperature: number;

  public lastRead: number;

  constructor() {
    this.lux = 3000;
    this.temperature = 5500;

    this.lastRead = Date.now();

    setInterval(() => this.update(), 700);
  }

  private update() {
    this.lux += (Math.random() - 0.5) * 20;
    this.temperature += (Math.random() - 0.5) * 5;

    this.lastRead = Date.now();
  }
}

const sensor = new Sensor();

export default sensor;
