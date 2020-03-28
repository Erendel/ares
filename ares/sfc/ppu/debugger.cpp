auto PPU::loadDebugger(Node::Object parent, Node::Object from) -> void {
  debugger.memory.vram = Node::append<Node::Memory>(parent, from, "PPU VRAM");
  debugger.memory.vram->setSize(vram.mask + 1 << 1);
  debugger.memory.vram->setRead([&](uint32 address) -> uint8 {
    return vram.data[address >> 1 & vram.mask].byte(address & 1);
  });
  debugger.memory.vram->setWrite([&](uint32 address, uint8 data) -> void {
    vram.data[address >> 1 & vram.mask].byte(address & 1) = data;
  });

  debugger.memory.oam = Node::append<Node::Memory>(parent, from, "PPU OAM");
  debugger.memory.oam->setSize(512 + 32);
  debugger.memory.oam->setRead([&](uint32 address) -> uint8 {
    return obj.oam.read(address);
  });
  debugger.memory.oam->setWrite([&](uint32 address, uint8 data) -> void {
    return obj.oam.write(address, data);
  });

  debugger.memory.cgram = Node::append<Node::Memory>(parent, from, "PPU CGRAM");
  debugger.memory.cgram->setSize(256 << 1);
  debugger.memory.cgram->setRead([&](uint32 address) -> uint8 {
    return dac.cgram[address >> 1 & 255].byte(address & 1);
  });
  debugger.memory.cgram->setWrite([&](uint32 address, uint8 data) -> void {
    dac.cgram[address >> 1 & 255].byte(address & 1) = data;
  });

  debugger.graphics.tiles2bpp = Node::append<Node::Graphics>(parent, from, "2 BPP Tiles");
  debugger.graphics.tiles2bpp->setSize(512, 512);
  debugger.graphics.tiles2bpp->setCapture([&]() -> vector<uint32_t> {
    vector<uint32_t> output;
    output.resize(512 * 512);
    for(uint tileY : range(64)) {
      for(uint tileX : range(64)) {
        uint15 address = tileY * 64 + tileX << 3;
        for(uint y : range(8)) {
          uint16 d0 = vram.data[address + y];
          for(uint x : range(8)) {
            uint2 color;
            color.bit(0) = d0.bit( 7 - x);
            color.bit(1) = d0.bit(15 - x);
            output[(tileY * 8 + y) * 512 + (tileX * 8 + x)] = color * 0x555555;
          }
        }
      }
    }
    return output;
  });

  debugger.graphics.tiles4bpp = Node::append<Node::Graphics>(parent, from, "4 BPP Tiles");
  debugger.graphics.tiles4bpp->setSize(512, 256);
  debugger.graphics.tiles4bpp->setCapture([&]() -> vector<uint32_t> {
    vector<uint32_t> output;
    output.resize(512 * 256);
    for(uint tileY : range(32)) {
      for(uint tileX : range(64)) {
        uint15 address = tileY * 64 + tileX << 4;
        for(uint y : range(8)) {
          uint16 d0 = vram.data[address + y + 0];
          uint16 d1 = vram.data[address + y + 8];
          for(uint x : range(8)) {
            uint4 color;
            color.bit(0) = d0.bit( 7 - x);
            color.bit(1) = d0.bit(15 - x);
            color.bit(2) = d1.bit( 7 - x);
            color.bit(3) = d1.bit(15 - x);
            output[(tileY * 8 + y) * 512 + (tileX * 8 + x)] = color * 0x111111;
          }
        }
      }
    }
    return output;
  });

  debugger.graphics.tiles8bpp = Node::append<Node::Graphics>(parent, from, "8 BPP Tiles");
  debugger.graphics.tiles8bpp->setSize(512, 128);
  debugger.graphics.tiles8bpp->setCapture([&]() -> vector<uint32_t> {
    vector<uint32_t> output;
    output.resize(512 * 128);
    for(uint tileY : range(16)) {
      for(uint tileX : range(64)) {
        uint15 address = tileY * 64 + tileX << 5;
        for(uint y : range(8)) {
          uint16 d0 = vram.data[address + y +  0];
          uint16 d1 = vram.data[address + y +  8];
          uint16 d2 = vram.data[address + y + 16];
          uint16 d3 = vram.data[address + y + 24];
          for(uint x : range(8)) {
            uint8 color;
            color.bit(0) = d0.bit( 7 - x);
            color.bit(1) = d0.bit(15 - x);
            color.bit(2) = d1.bit( 7 - x);
            color.bit(3) = d1.bit(15 - x);
            color.bit(4) = d2.bit( 7 - x);
            color.bit(5) = d2.bit(15 - x);
            color.bit(6) = d3.bit( 7 - x);
            color.bit(7) = d3.bit(15 - x);
            output[(tileY * 8 + y) * 512 + (tileX * 8 + x)] = color << 16 | color << 8 | color << 0;
          }
        }
      }
    }
    return output;
  });

  debugger.graphics.tilesMode7 = Node::append<Node::Graphics>(parent, from, "Mode 7 Tiles");
  debugger.graphics.tilesMode7->setSize(128, 128);
  debugger.graphics.tilesMode7->setCapture([&]() -> vector<uint32_t> {
    vector<uint32_t> output;
    output.resize(128 * 128);
    for(uint tileY : range(16)) {
      for(uint tileX : range(16)) {
        uint15 address = tileY * 16 + tileX << 6;
        for(uint y : range(8)) {
          for(uint x : range(8)) {
            uint8 color = vram.data[address + y * 8 + x].byte(1);
            output[(tileY * 8 + y) * 128 + (tileX * 8 + x)] = color << 16 | color << 8 | color << 0;
          }
        }
      }
    }
    return output;
  });
}