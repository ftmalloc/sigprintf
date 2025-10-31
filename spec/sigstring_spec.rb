require 'fiddle'

RSpec.describe "sigstring functions" do
  describe "sigstrlen" do
    it "returns the length of a string" do
      str = "Hello, world!"
      len = SigString.sigstrlen(str)
      expect(len).to eq(str.length)
    end

    it "returns the length of an empty string" do
      str = ""
      len = SigString.sigstrlen(str)
      expect(len).to eq(str.length)
    end
  end

  describe "sigstrcpy" do
    it "copies a string from one location to another" do
      str = "Hello, world!"
      buffer = Fiddle::Pointer.malloc(str.length + 1, Fiddle::RUBY_FREE)
      SigString.sigstrcpy(buffer, str)
      expect(buffer.to_s).to eq(str)
    end
  end

  describe "sigstrncpy" do
    it "copies a portion of a string from one location to another" do
      str = "Hello, world!"
      n_chars = 5
      buffer = Fiddle::Pointer.malloc(n_chars + 1, Fiddle::RUBY_FREE)
      SigString.sigstrncpy(buffer, str, n_chars)
      expect(buffer.to_s).to eq("Hello")
    end

    it "copies a string to the end of a buffer" do
      str = "Hello, world!"
      n_chars = 24
      buffer = Fiddle::Pointer.malloc(n_chars, Fiddle::RUBY_FREE)
      SigString.sigstrncpy(buffer, str, n_chars)
      expect(buffer.to_s).to eq(str)
    end
  end
end