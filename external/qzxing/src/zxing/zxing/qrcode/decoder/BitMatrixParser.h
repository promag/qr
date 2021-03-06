#ifndef ZXING_MATRIX_PARSER_H
#define ZXING_MATRIX_PARSER_H

/*
 *  BitMatrixParser.h
 *  zxing
 *
 *  Copyright 2010 ZXing authors All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <zxing/ReaderException.h>
#include <zxing/common/BitMatrix.h>
#include <QSharedPointer>
#include <zxing/qrcode/Version.h>
#include <zxing/qrcode/FormatInformation.h>

namespace zxing {
namespace qrcode {

class BitMatrixParser  {
private:
  QSharedPointer<BitMatrix> bitMatrix_;
  QSharedPointer<Version> parsedVersion_;
  QSharedPointer<FormatInformation> parsedFormatInfo_;
  bool mirror_;

  int copyBit(size_t x, size_t y, int versionBits);

public:
  BitMatrixParser(QSharedPointer<BitMatrix> bitMatrix);
  QSharedPointer<FormatInformation> readFormatInformation();
  QSharedPointer<Version>readVersion();
  QSharedPointer<std::vector<zxing::byte>> readCodewords();
  void remask();
  void setMirror(boolean mirror);
  void mirror();

private:
  BitMatrixParser(const BitMatrixParser&);
  BitMatrixParser& operator =(const BitMatrixParser&);
  
};

}
}

#endif // ZXING_MATRIX_PARSER_H
