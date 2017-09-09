#include "math/Quadrilateral.cpp"

#ifndef SQUARE_FINDER_CPP
#define SQUARE_FINDER_CPP

using namespace cv;
using namespace std;

/**
 * SquareFinder can be used to detect distorted squares in images.
 */
class SquareFinder
{
	public:
		/**
		 * Detect quads in grayscale image.
		 * @param gray Grayscale image.
		 * @param limitCosine Limit value for cosine in the quad corners, by default its 0.6.
		 * @returns sequence of squares detected on the image the sequence is stored in the specified memory storage
		 */
		static vector<Quadrilateral> findSquares(Mat gray, float limitCosine = 0.6, int minArea = 200)
		{
			//Quads found
			vector<Quadrilateral> squares = vector<Quadrilateral>();

			//Contours
			vector<vector<Point> > contours;

			//Find contours and store them all as a list
			findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
			vector<Point> approx;

			for(unsigned int i = 0; i < contours.size(); i++)
			{
				//Approximate contour with accuracy proportional to the contour perimeter
				approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);

				//Square contours should have 4 vertices after approximation relatively large area (to filter out noisy contours)and be convex.
				if(approx.size() == 4 && fabs(contourArea(Mat(approx))) > minArea && isContourConvex(Mat(approx)))
				{
					float maxCosine = 0;

					//Find the maximum cosine of the angle between joint edges
					for(int j = 2; j < 5; j++)
					{
						float cosine = fabs(angleCornerPointsCos(approx[j%4], approx[j-2], approx[j-1]));
						maxCosine = MAX(maxCosine, cosine);
					}

					//Check if all angle corner close to 90 (more than 60)
					if(maxCosine < limitCosine)
					{
						Quadrilateral quad = Quadrilateral();
						int j = 0;
						while(!approx.empty() && j < 4)
						{
							quad.points[j] = approx.back();
							approx.pop_back();
							j++;
						}
						squares.push_back(quad);
					}
				}
			}

			return squares;
		}

		/**
		 * Finds a cosine of angle between vectors from pt0->pt1 and from pt0->pt2.
		 *
		 * @param b Point b.
		 * @param c Point c.
		 * @param a Point a.
		 */
		static float angleCornerPointsCos(Point b, Point c, Point a)
		{
			float dx1 = b.x - a.x;
			float dy1 = b.y - a.y;
			float dx2 = c.x - a.x;
			float dy2 = c.y - a.y;

			return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-12);
		}
};

#endif