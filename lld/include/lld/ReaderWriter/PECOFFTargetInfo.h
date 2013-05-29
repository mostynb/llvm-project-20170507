//===- lld/ReaderWriter/PECOFFTargetInfo.h ---------------------------------===//
//
//                             The LLVM Linker
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLD_READER_WRITER_PECOFF_TARGET_INFO_H
#define LLD_READER_WRITER_PECOFF_TARGET_INFO_H

#include "lld/Core/TargetInfo.h"
#include "lld/ReaderWriter/Reader.h"
#include "lld/ReaderWriter/Writer.h"

#include "llvm/Support/COFF.h"
#include "llvm/Support/ErrorHandling.h"

namespace lld {

class PECOFFTargetInfo : public TargetInfo {
public:
  PECOFFTargetInfo() : _subsystem(llvm::COFF::IMAGE_SUBSYSTEM_UNKNOWN) {}

  virtual error_code parseFile(
      std::unique_ptr<MemoryBuffer> &mb,
      std::vector<std::unique_ptr<File>> &result) const;

  virtual Writer &writer() const;
  virtual bool validate(raw_ostream &diagnostics);

  virtual void addPasses(PassManager &pm) const {}

  void setSubsystem(llvm::COFF::WindowsSubsystem subsystem) {
    _subsystem = subsystem;
  }

  llvm::COFF::WindowsSubsystem getSubsystem() {
    return _subsystem;
  }

  virtual ErrorOr<Reference::Kind> relocKindFromString(StringRef str) const;
  virtual ErrorOr<std::string> stringFromRelocKind(Reference::Kind kind) const;

private:
  llvm::COFF::WindowsSubsystem _subsystem;

  mutable std::unique_ptr<Reader> _reader;
  mutable std::unique_ptr<Writer> _writer;
};

} // end namespace lld

#endif
