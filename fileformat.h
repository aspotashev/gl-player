#include <stdio.h>
#include <vector>

#pragma pack(push,4)
struct FileHeaderStruct
{
	int nParticles;
	int nFrames;
	float framesPerSecond;
};

struct VertexStruct
{
	float x, y, z;
};

struct EdgeStruct
{
	short int a, b;
};
#pragma pack(pop)


class FileFormat
{
public:
	FileFormat();
	~FileFormat();

	void openFile(const char *fn);
	void closeFile();
	void readFrame(int index,
		std::vector<VertexStruct> &v, std::vector<EdgeStruct> &e);
	int nFrames() const;

private:
	FILE *fd;
	FileHeaderStruct fileHeader;
	std::vector<long> frameOffsets;
};

