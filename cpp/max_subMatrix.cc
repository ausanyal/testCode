#include <iostream>
#include <ctime>

using namespace std;

typedef struct node_ {
	int width;
	int height;
}node_t;

class subMatrix {
private:
	int x;
	int y;
	int maxArea;
	int maxX;
	int maxY;
	int **arr;
	node_t **A;
	void getRandInp();
	void printMatrix();
	bool rightSame(int i, int j);
	bool bottomSame(int i, int j);
	int getWH(int i, int j);
public:
	subMatrix(int x, int y);
	~subMatrix();
	int getX();
	int getY();
	void processMatrix();
};

bool 
subMatrix::rightSame(int i, int j)
{
	// There is no right col since this is the last col
	if (j >= (this->getY() - 1)) {
		cout << __func__ << ": j " << j << " getY: " << this->getY() << endl;
		return false;
	} 
	
	// Right is not same as this
	if (this->arr[i][j] != this->arr[i][j+1]) {
		cout << __func__ << "arr[i][j]: " << this->arr[i][j] 
			 <<  " arr[i][j+1]: " << this->arr[i][j+1] << endl;
		return false;
	} 

	cout << __func__ << " i: " << i << " j: " << j << "  : return true" << endl;
	
	return true;
}

bool 
subMatrix::bottomSame(int i, int j)
{
	// There is no right col since this is the last col
	if (i >= (this->getX() - 1)) {
		cout << __func__ << ": i " << i << " getX: " << this->getX() << endl;
		return false;
	} 
	
	// Right is not same as this
	if (this->arr[i][j] != this->arr[i+1][j]) {
		cout << __func__ << "arr[i][j]: " << this->arr[i][j] 
			 <<  " arr[i+1][j]: " << this->arr[i+1][j] << endl;
		return false;
	} 

	cout << __func__ << " i: " << i << " j: " << j << "  : return true" << endl;

	return true;
}

int 
subMatrix::getWH(int i, int j)
{
	cout << "i: " << i << " j: " << j << " : " << arr[i][j] << endl;
	if (arr[i][j] == 0) {
		return -1;
	}

	bool rS = rightSame(i,j);
	bool bS = rightSame(i,j);
	
	// Case 1 : both right and bottom neighbor are different
	if (rS == false && bS == false) {
		A[i][j].width = A[i][j].height = 1;
		cout << "Case 1: ";
	}
	// Case 2 : right is same but bottom is different
	else if (rS == true && bS == false) {
		A[i][j].width 	= A[i][j+1].width + 1;
		A[i][j].height 	= 1;
		cout << "Case 2: ";
	}
	// Case 3 : right is different but bottom is same
	else if (rS == false && bS == true) {
		A[i][j].width 	= 1;
		A[i][j].height 	= A[i+1][j].height+1;
		cout << "Case 3: ";
	}
	// case 4 : both right and bottom are same
	else {
		node_t a, b, c;
		int aArea, bArea, cArea;
		a.width 	= A[i+1][j].width+1; 
		a.height 	= 1;
		aArea		= a.width * a.height;

		b.width 	= 1;
		b.height 	= A[i][j+1].height+1; 
		bArea		= b.width * b.height;

		c.width 	= min(A[i][j+1].width+1, 	A[i+1][j].width);
		c.height 	= min(A[i+1][j].height+1, 	A[i][j+1].height);
		cArea		= c.width * c.height;

		A[i][j] = ( (aArea > bArea) ? ( (aArea > cArea) ? a : c ) : 
									  ( (bArea > cArea) ? b : c ) );
		cout << "Case 4: ";
	}

	cout << "i: " << i << " j: " << j << " rS: " << rS << " bS: " << bS 
		<< " A[i][j].width/height = " << A[i][j].width << "/" << A[i][j].height << endl;

	return (A[i][j].width * A[i][j].height);
}

void
subMatrix::processMatrix()
{
	cout << " x: " << getX() << " y: " << getY() << endl;
	for (int i = this->x - 1 ; i >= 0 ; --i) {
		for (int j = this->y - 1 ; j >= 0 ; --j) {
			int area = getWH(i, j);
			if (area > this->maxArea) {
				this->maxArea = area;
				this->maxX = i;
				this->maxY = j;
			}
		}
	}
}

int main()
{
	subMatrix *a = new subMatrix(5, 5);
	a->processMatrix();

	return 0;
}

void
subMatrix::getRandInp()
{
	for(int i = 0 ; i < this->x ; ++i) {
		for(int j = 0 ; j < this->y ; ++j) {
			this->arr[i][j] = rand() % 2;
			this->A[i][j].width = 0;
			this->A[i][j].height = 0;
		}
	}
}

void
subMatrix::printMatrix()
{
	for(int i = 0 ; i < this->x ; ++i) {
		for(int j = 0 ; j < this->y ; ++j) {
			//cout << "arr[" << x << "][" << y << "] = " << this->arr[x][y];
			cout << this->arr[i][j] << " ";
		}
		cout << endl;
	}
}

subMatrix::subMatrix(int i, int j)
{
	this->x = i;
	this->y = j;
	this->maxArea = 0;
	this->maxX = this->maxY = -1;
	this->arr 	= (int **)malloc(this->x * sizeof(int *));
	this->A 	= (node_t **)malloc(this->x * sizeof(node_t *));
	srand(time(0));
	for (int i = 0; i < this->x ; ++i) {
			this->arr[i] 	= (int *)malloc(this->y * sizeof(int));
			this->A[i] 		= (node_t *)malloc(this->y * sizeof(node_t));
	}	
	this->getRandInp();
	this->printMatrix();
}

subMatrix::~subMatrix()
{
}

int
subMatrix::getX()
{
	return this->x;
}

int
subMatrix::getY()
{
	return this->y;
}
