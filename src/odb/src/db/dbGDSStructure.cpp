///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2022, The Regents of the University of California
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

// Generator Code Begin Cpp
#include "dbGDSStructure.h"

#include "dbDatabase.h"
#include "dbDiff.hpp"
#include "dbTable.h"
#include "dbTable.hpp"
#include "odb/db.h"
#include "odb/dbTypes.h"
namespace odb {
template class dbTable<_dbGDSStructure>;

bool _dbGDSStructure::operator==(const _dbGDSStructure& rhs) const
{
  if (_strname != rhs._strname) {
    return false;
  }

  return true;
}

bool _dbGDSStructure::operator<(const _dbGDSStructure& rhs) const
{
  return true;
}

void _dbGDSStructure::differences(dbDiff& diff,
                                  const char* field,
                                  const _dbGDSStructure& rhs) const
{
  DIFF_BEGIN
  DIFF_FIELD(_strname);
  DIFF_END
}

void _dbGDSStructure::out(dbDiff& diff, char side, const char* field) const
{
  DIFF_OUT_BEGIN
  DIFF_OUT_FIELD(_strname);

  DIFF_END
}

_dbGDSStructure::_dbGDSStructure(_dbDatabase* db)
{
}

_dbGDSStructure::_dbGDSStructure(_dbDatabase* db, const _dbGDSStructure& r)
{
  _strname = r._strname;
}

dbIStream& operator>>(dbIStream& stream, _dbGDSStructure& obj)
{
  stream >> obj._strname;
  stream >> obj._elements;
  return stream;
}

dbOStream& operator<<(dbOStream& stream, const _dbGDSStructure& obj)
{
  stream << obj._strname;
  stream << obj._elements;
  return stream;
}

////////////////////////////////////////////////////////////////////
//
// dbGDSStructure - Methods
//
////////////////////////////////////////////////////////////////////

}  // namespace odb
   // Generator Code End Cpp