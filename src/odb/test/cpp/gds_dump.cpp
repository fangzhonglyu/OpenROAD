#include <odb/db.h>
#include <odb/gdsin.h>
#include <odb/gdsout.h>

int main(int argc, char* argv[])
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <input file> <output file>"
              << std::endl;
    return 1;
  }
  odb::gds::GDSReader reader;
  odb::dbDatabase* db = odb::dbDatabase::create();
  odb::dbGDSLib* lib = reader.read_gds(argv[1], db);

  odb::gds::GDSWriter writer;
  writer.dump_gds_content(lib, argv[2]);

  delete lib;
}
