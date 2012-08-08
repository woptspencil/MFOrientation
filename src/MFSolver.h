//
//  MFSolver.h
//  MatrixFactorizationOrientation
//
//  Created by Jerry Ni on 12/7/29.
//  Copyright (c) 2012年 woptspencil@gmail.com. All rights reserved.
//

#ifndef MatrixFactorizationOrientation_MFSolver_h
#define MatrixFactorizationOrientation_MFSolver_h

#include "Data.h"

#define TRAIN_DATA		"./data/train.data"
#define VALIDATION_DATA	"./data/val.data"

class MFSolver {
public:
	MFSolver(Data *d);
	void Run();
	
	double Predict(User user, Item item);
	void TrainOnRating(User user, Item item);
	double RMSE(RatingType type);	
	void Print(int currentIteration);
	
protected:
	Data *data;
	int featureSize;
	int iterations;
	
	double **q;
	double **p;	
};

#endif
