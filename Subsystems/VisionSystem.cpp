/*#include "VisionSystem.h"

VisionSystem::VisionSystem()

{
	m_camera->GetInstance("10.32.34.11");
	m_camera->WriteResolution(AxisCamera::kResolution_320x240);
	m_camera->WriteCompression(20);
	m_camera->WriteBrightness(0);
	m_camera->WriteColorLevel(50);
	Wait(3.0);

}

VisionSystem::~VisionSystem()
{

}

void VisionSystem::imageProcessor()
{
	Threshold threshold(60, 100, 90, 255, 20, 255);
	ParticleFilterCriteria2 criteria[] = {
			{IMAQ_MT_AREA, AREA_MINIMUM, 65535, false, false}
	};
	ColorImage *image;
	image = new RGBImage("/testImage.jpg");	//get the sample image from the cRIO flash
	//camera.GetImage(image)				//To get the images from the camera coment the line above and uncoment this one
	BinaryImage *thresholdImage = image->ThresholdHSV(threshold);	//just get the green pixels
	//thresholdImage->Write("threshold.bmp");
	BinaryImage *convexHullImage = thresholdImage->ConvexHull(false);	//fill in partial and full rectangles
	//convexHullImage->Write("ConvexHull.bmp");
	BinaryImage *filteredImage = convexHullImage->ParticleFilter(criteria, 1);	//remove small particles
	//filteredImage->Write("Filtered.bmp");
	
	vector<ParticleAnalysisReport> *reports = filteredImage->GetOrederedParticleAnalysisReports();
	scores = new Scores[reports->size()];
	
	for (unsigned i = 0; i < reports->size(); i++) {
		ParticleAnalysisReport *report = &(reports->at(i));
		
		scores(i).rectangularity = scoreRectangularity(report);
		score(i).aspectRatioOuter = scoreAspectRatio(filteredImage, report, true);
		score(i).aspectRatioInner = scoreAspectRatio(filteredImage, report, false);
		score(i).xEdge = scoreXEdge(thresholdImage, report);
		score(i).yEdge = scoreYEdge(thresholdImage, report);
		
		if(scoreCompare(scores[i], false))
		{
			printf("particle:%d is a High Goal centerX: %f centerY: %f \n", i, report->center_mass_x_normalized, report->center_mass_y_normalized);
			printf("Distance: %f \n", computerDistance(thresholdImage, report, false));
		} else {
			printf("particle: %d is not a goal centerX: %f centerY %f \n", i, report->center_mass_x_normalized, report->center_mass_y_normalized);
		}
		printf("rect: %f ARinner: %f \n", scores[i].rectangularity, scores[i].xEdge, scores{i}.yEdge);
	}
	
}

void VisionSystem::Reset()
{

}

	
bool VisionSystem::Update()
{
	
}

double VisionSystem::scoreAspectRatio(BinaryImage *image, ParticleAnalysisReport *report, bool vertical){
		double rectLong, rectShort, idealAspectRatio, aspectRatio;
		idealAspectRatio = vertical ? (4.0/32) : (23.5/4); //Vertical reflector 4" wide x 32" tall, horizontal 23.5" wide x 4" tall
		
		imaqMeasureParticle(image->GetImaqImage(), report->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &rectLong);
		imaqMeasureParticle(image->GetImaqImage(), report->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &rectShort);
		
		//Divide width by height to measure aspect ratio
		if(report->boundingRect.width > report->boundingRect.height){
			//particle is wider than it is tall, divide long by short
			aspectRatio = ratioToScore(((rectLong/rectShort)/idealAspectRatio));
		} else {
			//particle is taller than it is wide, divide short by long
			aspectRatio = ratioToScore(((rectShort/rectLong)/idealAspectRatio));
		}
		return aspectRatio; 		//force to be in range 0-100
		
	}
double VisionSystem::scoreRectangularity(ParticleAnalysisReport *report){
	if(report->boundingRect.width*report->boundingRect.height !=0){
		return 100*report->particleArea/(report->boundingRect.width*report->boundingRect.height);
	} else {
		return 0;
	}
}

double ratioToScore(double ratio)
{
	return (max(0, min(100*(1-fabs(1-ratio)), 100)));
}

bool scoreCompare(Scores scores, bool vertical){
	bool isTarget = true;
	
	isTarget &= scores.rectangularity > RECTANGULARITY_LIMIT;
	if(vertical){
		isTarget &= scores.aspectRatioVertical > ASPECT_RATIO_LIMIT;
	} else {
		isTarget &= scores.aspectRatioHorizontal > ASPECT_RATIO_LIMIT;
	}
	
	return isTarget;

}

*/
