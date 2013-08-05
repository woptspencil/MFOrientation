//
//  MFSolver.cpp
//  MatrixFactorizationOrientation
//
//  Created by Jerry Ni on 12/7/29.
//  Copyright (c) 2012年 woptspencil@gmail.com. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <cstdlib>
#include "MFSolver.h"

int main(int argc, char *argv[]) {	
	Data *data = new Data(TRAIN_DATA, VALIDATION_DATA);
	MFSolver *solver = new MFSolver(data);

	solver->Run();
	
	delete data;
	delete solver;
	
	return 0;
}

MFSolver::MFSolver(Data *d) {
	data = d;
	
	// Variables
	featureSize = 3;
	iterations = 1;

	// Random initial q
	q = new double *[data->getItemCount()];
	for (int i = 0; i < data->getItemCount(); i++) {
		q[i] = new double[featureSize];
		for (int f = 0; f < featureSize; f++) {
			q[i][f] = rand() % 1000 / 1000.0;
		}
	}

	// Random initial p
	p = new double *[data->getUserCount()];
	for (int u = 0; u < data->getUserCount(); u++) {
		p[u] = new double[featureSize];
		for (int f = 0; f < featureSize; f++) {
			p[u][f] = rand() % 1000 / 1000.0;
		}
	}	
}

void MFSolver::Run() {
	double bestValRMSE = INFINITY;
	int bestValIteration = -1;
	
	// Iteratively run SGD
	for (int it = 0; it < iterations; it++) {
		// Get next user from data
		User *u = data->getNextUser();
		while (u != NULL) {
			// For every item of the user
			for (Item i = Item(0); i != Item(ItemCount); i = Item(i+1)) {
				// Check if there's rating for item				
				if (u->ratings[i].score == UNKNOWN || u->ratings[i].type == Validation) {
					continue;
				}
				
				// Train on this rating
				TrainOnRating(*u, i);
			}
			
			// Get next user
			u = data->getNextUser();
		}
		
		// End of this iteration
		Print(it);
		double valRMSE = RMSE(Validation);
		if (valRMSE < bestValRMSE) {
			bestValRMSE	= valRMSE;
			bestValIteration = it;
		}
	}
	
	// Print best result
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(2);
	cout << ASCII_RED << "========================================= ";
	cout << "Final Results ";
	cout << " =========================================\n" << ASCII_RESET;
	cout << "Final Train RMSE = " << RMSE(Training) << endl;
	cout << ASCII_PURPLE << "Final Validation RMSE = " << RMSE(Validation) << ASCII_RESET << endl;
	cout << ASCII_YELLOW << "Best Validation RMSE = " << bestValRMSE;
	cout << " (on iteration " << bestValIteration + 1 << ")\n" << ASCII_RESET;
	cout.unsetf(ios::floatfield);
}

double MFSolver::Predict(User user, Item item) {
	int u = user.u_id;
	int i = item;
	
	// Inner product of qi and pu
	double result = 0;
	for (int k = 0; k < featureSize; k++) {
		result += q[i][k] * p[u][k];
	}
		
	return result;
}

void MFSolver::TrainOnRating(User user, Item item) {
/*	int itemId = item;
	int userId = user.u_id;

	double score = user.ratings[item].score;
	double prediction = Predict(user, item);
	double error = score - prediction; 
 
	double mu = data->getGlobalMean();
 	double b_u = data->getUserMean(user) - mu;
 	double b_i = data->getItemMean(item) - mu;
*/
	
	// TODO: Train qi and pu right here!
	
	return;
}

double MFSolver::RMSE(RatingType type) {
	double result = 0;
	User *u = data->getNextUser();
	int count = 0;
	while (u != NULL) {
		for (Item i = Item(0); i != Item(ItemCount); i = Item(i+1)) {
			if (u->ratings[i].score == UNKNOWN || u->ratings[i].type != type) {
				continue;
			}
			
			double p = Predict(*u, i);
			count ++;
			result += (p - u->ratings[i].score) * (p - u->ratings[i].score);
		}
		
		// Get next user
		u = data->getNextUser();
	}
	
	return sqrt(result / count);
}

void MFSolver::Print(int currentIteration) {
	cout << ASCII_RED << "========================================= ";
	cout << "Iteration " << currentIteration + 1;
	cout << " =========================================\n" << ASCII_RESET;
	
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(2);

	// Print qi
	if (featureSize <= data->getUserCount()) {
		cout << ASCII_CYAN << "[qi]" << ASCII_RESET << endl;
		for (int i = 0; i < (5 * featureSize - 3) / 2 + 8; i++) cout << " ";
		cout << ASCII_YELLOW << "Feature" << ASCII_RESET << endl;
		for (int i = 0; i < data->getItemCount(); i++) {
			if (i == data->getItemCount() / 2) {
				cout << ASCII_YELLOW << "  Item  " << ASCII_RESET;
			} else {
				cout << "        ";
			}
			
			cout << "|";
			for (int f = 0; f < featureSize; f++) {
				cout.width(5);
				cout << q[i][f];
			}
			cout << " |\n";
		}
	}
	
	// Print pu
	if (featureSize <= data->getUserCount()) {
		cout << ASCII_CYAN << "[pu]" << ASCII_RESET << endl;
		for (int i = 0; i < (5 * data->getUserCount()) / 2 + 8; i++) cout << " ";
		cout << ASCII_YELLOW << "User" << ASCII_RESET << endl;
		for (int f = 0; f < featureSize; f++) {
			if (f == featureSize / 2) {
				cout << ASCII_YELLOW << "Feature " << ASCII_RESET;
			} else {
				cout << "        ";
			}
			
			cout << "|";
			for (int u = 0; u < data->getUserCount(); u++) {
				cout.width(5);
				cout << p[u][f];
			}
			cout << " |\n";
		}
		cout << endl;
	}
	
	// Print prediction
	cout << ASCII_CYAN << "[Prediction]" << ASCII_RESET << endl;
	cout << "  White: Train Set" << endl;
	cout << ASCII_PURPLE << "  Purple: Validation Set" << ASCII_RESET << endl;
	cout << ASCII_YELLOW << "              ";
	for (Item i = Item(0); i < Item(ItemCount); i = Item(i+1)) {
		if (Data::ItemName(i).length() < 5) cout.width(5);
		cout << Data::ItemName(i);
		cout << "  ";
	}
	cout << ASCII_RESET << endl;
	User *u = data->getNextUser();
	while (u != NULL) {
		
		cout << ASCII_GREEN;
		cout.width(12);
		cout << u->name << ASCII_RESET;
		
		for (Item i = Item(0); i < Item(ItemCount); i = Item(i+1)) {
			int length = Data::ItemName(i).length();
			if (length < 5) length = 5;

			Rating r = u->ratings[i];
			if (r.score < 0) {
				cout.width(length + 2);
				cout << "";
			} else if (r.type == Training) {
				cout.width(length);
				cout << Predict(*u, i);
				cout << ":" << r.score;
			} else {
				cout << ASCII_PURPLE;
				cout.width(length);
				cout << Predict(*u, i);
				cout << ":" << r.score;
				cout << ASCII_RESET;
			}
		}
		cout << endl;
		
		u = data->getNextUser();
	}
	cout << endl;
	
	// Print results
	cout << ASCII_CYAN << "[Results]" << ASCII_RESET << endl;
	cout << "  Training RMSE = " << RMSE(Training) << endl;
	cout << ASCII_PURPLE << "  Validation RMSE = " << RMSE(Validation) << ASCII_RESET << endl;
	cout << endl;
	
	cout.unsetf(ios::floatfield);
}
