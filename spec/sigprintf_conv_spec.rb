require 'fiddle'

RSpec.describe "sigprintf conversion functions" do
  describe "is_little_endian" do
    it "tests the endianness of the host system" do
      endianness = [1].pack("N") == [1].pack("I") ? 0 : 1
      result = SigPrintfConv.is_little_endian
      expect(result).to eq(endianness)
    end
  end

  describe "sigultoa" do
    it "converts an unsigned long integer to a string" do
      b = Fiddle::Pointer.malloc(24, Fiddle::RUBY_FREE)

      SigPrintfConv.sigultoa(b, 12345)
      expect(b.to_s).to eq("12345")

      SigPrintfConv.sigultoa(b, 123456)
      expect(b.to_s).to eq("123456")
    end

    it "converts 0 to a string" do
      b = Fiddle::Pointer.malloc(24, Fiddle::RUBY_FREE)

      SigPrintfConv.sigultoa(b, 0)
      expect(b.to_s).to eq("0")
    end
  end

  describe "sigltoa" do
    it "converts a positive signed long integer to a string" do
      b = Fiddle::Pointer.malloc(24, Fiddle::RUBY_FREE)

      SigPrintfConv.sigltoa(b, 12345)
      expect(b.to_s).to eq("12345")

      SigPrintfConv.sigltoa(b, 123456)
      expect(b.to_s).to eq("123456")
    end

    it "converts a negative signed long integer to a string" do
      b = Fiddle::Pointer.malloc(24, Fiddle::RUBY_FREE)

      SigPrintfConv.sigltoa(b, -12345)
      expect(b.to_s).to eq("-12345")

      SigPrintfConv.sigltoa(b, -123456)
      expect(b.to_s).to eq("-123456")
    end

    it "converts 0 to a string" do
      b = Fiddle::Pointer.malloc(24, Fiddle::RUBY_FREE)

      SigPrintfConv.sigltoa(b, 0)
      expect(b.to_s).to eq("0")
    end
  end

  describe "sigptoa" do
    it "converts a pointer to a string" do
      b = Fiddle::Pointer.malloc(24, Fiddle::RUBY_FREE)
      p = Fiddle::Pointer.malloc(24, Fiddle::RUBY_FREE)

      SigPrintfConv.sigptoa(b, p)
      expect(b.to_s).to eq("0x#{p.to_i.to_s(16)}")
    end

    it "converts a null pointer to a string" do
      b = Fiddle::Pointer.malloc(24, Fiddle::RUBY_FREE)

      SigPrintfConv.sigptoa(b, 0)
      expect(b.to_s).to eq("0x0")
    end
  end

  describe "sightoa" do
    it "converts an unsigned integer to a string in hexadecimal format" do
      b = Fiddle::Pointer.malloc(24, Fiddle::RUBY_FREE)

      SigPrintfConv.sightoa(b, 42)
      expect(b.to_s).to eq("0x2a")

      SigPrintfConv.sightoa(b, 65535)
      expect(b.to_s).to eq("0xffff")

      SigPrintfConv.sightoa(b, 0)
      expect(b.to_s).to eq("0x0")
    end
  end
end