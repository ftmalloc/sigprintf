require 'fiddle'
require 'fiddle/import'

LibPath = File.expand_path(/darwin/ =~ RUBY_PLATFORM ?
                             './cmake-build-debug/libsigprintf_test.dylib' :
                             './cmake-build-debug/libsigprintf_test.so')

module SigPrintfConv
  extend Fiddle::Importer
  dlload LibPath

  extern 'int is_little_endian(void)'
  extern 'void sigultoa(char *, unsigned long)'
  extern 'void sigltoa(char *, long)'
  extern 'void sigptoa(char *, void *)'
  extern 'void sightoa(char *, unsigned long)'
end

module SigString
  extend Fiddle::Importer
  dlload LibPath

  extern 'size_t sigstrlen(const char *s)'
  extern 'char *sigstrcpy(char *dst, const char *src)'
  extern 'char *sigstrncpy(char *dst, const char *src, size_t len)'
  extern 'char *sigstrchr(const char *s, int c)'
  extern 'void *sigmemset(void *s, int c, size_t n)'
  extern 'void *sigmemcpy(void *dst, const void *src, size_t n)'
  extern 'void *sigmemmove(void *dst, const void *src, size_t n)'
end

RSpec.configure do |config|
  config.expect_with :rspec do |expectations|
    expectations.include_chain_clauses_in_custom_matcher_descriptions = true
  end

  config.mock_with :rspec do |mocks|
    mocks.verify_partial_doubles = true
  end

  config.shared_context_metadata_behavior = :apply_to_host_groups
end
