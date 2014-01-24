/*#ifndef __First_RedArrow__VisionSystem__
#define __First_RedArrow__VisionSystem__
#define Y_IMAGE_RES 240
#define VIEW_ANGLE 43.5
#define PI 3.141592653

#define RECTANGULARITY_LIMIT 40
#define ASPECT_RATIO_LIMIT 55

#define TAPE_WIDTH_LIMIT 50
#define VERTICAL_SCORE_LIMIT 50
#define LR_SCORE_LIMIT 50

#define AREA_MINIMUM 150

#define MAX_PARTICLES 8




class VisionSystem
{
public:
	VisionSystem();
	~VisionSystem();
	
	void imageProcessor();
	
	void Reset();
	
	
	bool Update();
private:
	double scoreAspectRatio(BinaryImage *image, ParticleAnalysisReport *report, bool vertical);
	double scoreRectangularity(ParticleAnalysisReport *report);
	double ratioToScore(double ratio);
	bool scoreCompare(Scores scores, bool vertical);
	
	
private:
	AxisCamera *m_camera;
	struct Scores {
		double rectangularity;
		double aspectRatioVertical;
		double aspectRatioHorizontal;
	};
	struct targetReport{
		int verticalIndex;
		int horizontalIndex;
		bool Hot;
		double totalScore;
		double leftScore;
		double rightScore;
		double tapeWidthScore;
		double verticalScore;
	};
	
};


#endif
*/
