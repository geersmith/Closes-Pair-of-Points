/*
 * Name: Geer Smith
 * Date Submitted: 
 * Lab Section:
 * Assignment Name: Lab 6
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>

using namespace std;

struct point
{
    double x;
    double y;
};
//chose sqrt of points as b to get close to input size with this certain amount of panes
int gridSize(int numPoints){
    return sqrt(numPoints);
}
double findDist(point first, point second){
    double valX=(second.x-first.x);
    valX = pow(valX, 2);
    double valY=(second.y-first.y);
    valY = pow(valY, 2);
    return sqrt(valX+valY);
}
vector<vector<vector<point> > > grid;
double shortDist = 1000.0;
vector<point> getPoints(int row, int column){
    return grid[row][column];
}

void pointChecker(point curr, vector<point> toCheck){
    int j;
    for(j = 0; j < toCheck.size(); j++){
        double distBetween = findDist(curr, toCheck[j]);
        if(distBetween < shortDist){
            shortDist = distBetween;
        }
    }
}

/*Implement the following function
  Reads in a file specified by the parameter
  Format of file: #of points on first line
                  remaining (#of points) lines: x-value and y-value of point
                  one point per line
                  x-value and y-value are double-precision values and
                  bounded by the unit square 0.0 <= x,y < 1.0
  Should use "spatial hashing" where the #of cells scales with the #of points
  and find the distance between the closest pair of points which will be
  returned as a double type value
*/
double closestPair(string filename){
    ifstream infile(filename);
    string str;
    getline(infile, str);
    int pointNum = stoi(str);
    int mainGrid = gridSize(pointNum);
    double inter = 1.0 / mainGrid;
    string insertPoint;
    grid = vector<vector<vector<point> > >(mainGrid,vector<vector<point> >(
                                         mainGrid,vector <point>(0)));
    while(getline(infile, insertPoint)){
        istringstream stream(insertPoint);
        string strX;
        string strY;
        stream >> strX >> strY;
        point valPoint;
        valPoint.x=stod(strX);
        valPoint.y=stod(strY);
        int posX=valPoint.x/inter;
        int posY=valPoint.y/inter;
        grid[posX][posY].push_back(valPoint);
    }
    for(int j = 0; j <mainGrid; j++){
        for(int k = 0; k < mainGrid; k++){
            vector<point> inCell = getPoints(j,k);
            if(inCell.size()>1){
                for(int current = 0; current < inCell.size(); current++){
                    vector<point> subOfPoints(inCell.begin() + current + 1, 
                                              inCell.end());
                    pointChecker(inCell[current], subOfPoints); 
                }
            }
            if(k + 1 < mainGrid){
                vector<point> rightPoints = getPoints(j, k + 1);
                for(int current = 0; current < inCell.size(); current++){
                    pointChecker(inCell[current], rightPoints);
                }
            }
            if((k - 1)>0 && (j + 1) < mainGrid){
                vector<point> leftDiagPoints = getPoints(j + 1, k - 1);
                for(int current = 0; current < inCell.size(); current++){
                    pointChecker(inCell[current], leftDiagPoints);
                }
            }
            if(j + 1 < mainGrid){
                vector<point> downPoints = getPoints(j + 1, k);
                for(int current = 0; current < inCell.size(); current++){
                    pointChecker(inCell[current], downPoints);
                }
            }
            if(k + 1 < mainGrid && j + 1 < mainGrid){
                vector<point> rightDiagPoints = getPoints(j + 1, k + 1);
                for(int current = 0; current < inCell.size(); current++){
                    pointChecker(inCell[current], rightDiagPoints);
                }
            }
        }
    }
    return shortDist;
}

int main()
{
    double min;
    string filename;
    cout << "File with list of points within unit square: ";
    cin >> filename;
    min = closestPair(filename);
    cout << setprecision(16);
    cout << "Distance between closest pair of points: " << min << endl;
    return 0;
}
