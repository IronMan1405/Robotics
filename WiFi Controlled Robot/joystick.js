var joystick = nipplejs.create({
  zone: document.getElementByIgetElementById("joystick-zone"),
  mode: "static",
  position: { left: "50%", top: "50%" },
  color: "grey",
});

joystick.on("dir", function (evt, data) {
  if (data.direction) {
    const dir = data.direction.angle;
    console.log("Direction: ", dir);

    siwtch(dir) {
      case 'up':
      fetch("/forward");
      break;
      case 'down':
      fetch("/reverse");
      break;
      case 'right':
      fetch("/right");
      break;
      case 'left':
      fetch("/left");
      break;
    }
  }
});

joystick.on('end', function () {
  fetch('/stop');
});
