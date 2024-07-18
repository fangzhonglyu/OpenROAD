#include <iostream>
#include <string>

// #include "odb/db.h"

#include "odb/gdsin.h"
#include "odb/gdsout.h"

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
    return 1;
  }
  odb::GDSReader reader;
  odb::dbDatabase* db = odb::dbDatabase::create();
  odb::dbGDSLib* lib = reader.read_gds(argv[1], db);
  std::cout << "Library: " << lib->getLibname() << std::endl;
  std::cout << "Units: " << lib->getUnits().first << " " << lib->getUnits().second << std::endl;

  odb::GDSWriter writer;
  writer.write_gds(lib, argv[2]);

  delete lib;
}
