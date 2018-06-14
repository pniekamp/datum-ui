//
// datum-ui - compiler
//

//
// Copyright (c) 2018 Peter Niekamp
//

#include "parser.h"
#include "generator.h"
#include <leap.h>
#include <leap/pathstring.h>
#include <iostream>

using namespace std;
using namespace leap;

///////////////////////// compile ///////////////////////////////////////////
void compile(pathstring const &src, pathstring const &dst, string const &classname)
{
  cout << "  Compiling: " << src.name() << endl;

  auto txt = load_ui(src);

  cout << "    Source: " << txt.size() << " bytes" << endl;

  auto bytecode = parse_ui(txt);

  cout << "    ByteCode: " << bytecode.size() << " bytes" << endl;

  codegen_ui(classname, bytecode.data(), bytecode.size(), dst);
}


///////////////////////// main //////////////////////////////////////////////
int main(int argc, char **argv)
{
  cout << "Datum Ui Compiler" << endl;

  string srcfile;

  for(int i = 0, j = 0; i < argc; ++i)
  {
    if (argv[i][0] == '-')
      continue;

    if (j == 1)
      srcfile = argv[i];

    ++j;
  }

#ifdef _WIN32
  replace(begin(srcfile), end(srcfile), '\\', '/');
#endif

  if (srcfile == "")
  {
    cout << "ui filename missing." << endl;
    exit(1);
  }

  try
  {
    auto src = pathstring(".", srcfile);
    auto dst = pathstring(".", src.basename() + "_p.h");
    auto classname = toupper(src.basename().substr(0, 1)) + src.basename().substr(1);

    compile(src, dst, classname);
  }
  catch(exception &e)
  {
    cerr << "Critical Error: " << e.what() << endl;
  }

  return 0;
}
