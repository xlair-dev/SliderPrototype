# include <Siv3D.hpp> // Siv3D v0.6.14

void Main() {
    constexpr size_t Width = 16;
    constexpr size_t SensorSize = Width * 2;
    constexpr int32 Baudrate = 115200;

    const Array<SerialPortInfo> infos = System::EnumerateSerialPorts();
    const Array<String> options = infos.map([](const SerialPortInfo& info) {
        return U"{} ({})"_fmt(info.port, info.description);
    }) << U"none";

    Serial serial;
    size_t index = (options.size() - 1);

    Array<bool> state(SensorSize);
    Array<bool> key(SensorSize);
    int32 t = 0;

    while (System::Update()) {
        const bool isOpen = serial.isOpen();

        if (SimpleGUI::RadioButtons(index, options, Vec2(200, 60))) {
            ClearPrint();

            if (index == (options.size() - 1)) {
                serial = Serial();
            } else {
                Print << U"Open {}"_fmt(infos[index].port);

                // Open Serial Port
                if (serial.open(infos[index].port, Baudrate)) {
                    Print << U"Succeeded";
                } else {
                    Print << U"Failed";
                }
            }
        }

        if (const size_t available = serial.available()) {
            const auto line = serial.readBytes();
            Print << line;

            for (const auto c : line) {
                if (c == '\n') {
                    // separator
                    ClearPrint();
                    if (t >= SensorSize) {
                        for (uint32 i = 0; i < SensorSize; i++) {
                            key[i] = state[i];
                        }
                    }
                    t = 0;
                } else if (c == '0') {
                    // not pressed
                    if (t >= SensorSize) {
                        continue;
                    }
                    state[t] = false;
                    t++;
                } else if (c == '1') {
                    // pressed
                    if (t >= SensorSize) {
                        continue;
                    }
                    state[t] = true;
                    t++;
                } else {
                    // invalid
                }
            }

            const double width = static_cast<double>(Scene::Width()) / Width;
            const double height = static_cast<double>(Scene::Height()) / 2.0;
            for (uint32 i = 0; i < Width; i++) {
                const auto top = i * 2;
                const auto btm = i * 2 + 1;
                if (key[top]) {
                    RectF(width * i, 0, width, height).draw(Palette::Red);
                }
                if (key[btm]) {
                    RectF(width * i, height, width, height).draw(Palette::Red);
                }
            }
        }
    }
}
