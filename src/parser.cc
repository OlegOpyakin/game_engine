#include "parser.h"

bool LoadFromObjectFile(std::string filename, std::vector<triangle> *tris) //const char *filename
{
	std::ifstream f(filename, std::ios::in);
	if (!f){
        perror("file");
        exit(1);
    }

	std::vector<vec3d> verts;

	while (!f.eof())
	{
		char line[128];
		f.getline(line, 128);

		std::stringstream s;
		s << line;

		char junk;

		if (line[0] == 'v')
		{
			vec3d v;
			s >> junk >> v.x >> v.y >> v.z;
			verts.push_back(v);
		}

		if (line[0] == 'f')
		{
			int f[3];
			s >> junk >> f[0] >> f[1] >> f[2];
			triangle tr = { verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] };
			tris->push_back(tr);
		}
	}

	return true;
}
