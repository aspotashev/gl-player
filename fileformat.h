#include <stdio.h>
#include <vector>
#include <QtGui>

#pragma pack(push,4)
struct FileHeaderStruct
{
	int fileVersion;
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

	void openFile(const QString &fn);
	void closeFile();
	void readFrame(int index,
		std::vector<VertexStruct> &v, std::vector<EdgeStruct> &e);
	int nFrames() const;
	int nVertices() const;
	void fileSeekFrame(int index);
	std::vector<float> loadPressureData();

private:
	QFile *fd;

	FileHeaderStruct fileHeader;
	std::vector<qint64> frameOffsets;
};

