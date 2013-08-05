//
//  Data.cpp
//  MatrixFactorizationOrientation
//
//  Created by 倪 嘉懋 on 12/7/25.
//  Copyright (c) 2012年 woptspencil@gmail.com. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Data.h"

User::User(string theName, int num) {
	name = theName;
	u_id = num;
		
	for (Item i = Item(0); i < Item(ItemCount); i = Item(i+1)) {
		ratings[i].score = UNKNOWN;
		ratings[i].type = Training;
	}
};

string Data::ItemName(Item item) {
	switch (item) {
		case Titanic:		return string("Titanic");	
		case Inception:		return string("Inception");	
		case Avengers:		return string("Avengers");	
		case TheDarkKnight:	return string("TheDarkKnight");	
		case Ted:			return string("Ted");	
		case GodFather:		return string("GodFather");	
		case SpiderMan:		return string("SpiderMan");	
		case Matrix:		return string("Matrix");
		default:
			cerr << "ERROR: Try to get invalid item name\n";
			exit(-1);
	} 
}

Data::Data(const char *trainFile, const char *valFile) {
	// Train File
	ifstream ftrain(trainFile);
	
	if (!ftrain.good()) {
		cerr << "ERROR: Data file not found\n";
		exit(-1);
	}
	
	// Read in
	string name, movie;
	double sum = 0;
	int count = 0;
	int rating;
	userCount = 0;
	ftrain >> name >> movie >> rating;
	while (ftrain.good()) {
		if (!(rating > 0 && rating < 6)) {
			cerr << "ERROR: Invalid rating: " << rating << endl;
			exit(-1);
		}
		
		map<string, User>::iterator it = users.find(name);
		if (it == users.end()) {
			User u(name, userCount);
			users.insert(pair<string, User>(name, u));
			userCount++;
		}		
		it = users.find(name);
		
		Item item;
		bool legalName = false;
		for (Item i = Item(0); i < Item(ItemCount); i = Item(i+1)) {
			if (!movie.compare(Data::ItemName(i))) {
				item = i;
				legalName = true;
			}
		}
		if (!legalName) {
			cerr << "ERROR: Can't recognize movie: " << movie << endl;
			exit(-1);
		}
		it->second.ratings[item].score = rating;
		it->second.ratings[item].type = Training;
		sum += rating;
		count++;
		
		ftrain >> name >> movie >> rating;		
	}
	mu = count > 0 ? sum / count : 0.0;
	
	// Validation File
	ifstream fval(valFile);
	
	if (!fval.good()) {
		cerr << "ERROR: Data file not found\n";
		exit(-1);
	}
	
	// Read in
	fval >> name >> movie >> rating;
	while (fval.good()) {
		if (!(rating > 0 && rating < 6)) {
			cerr << "ERROR: Invalid rating: " << rating << endl;
			exit(-1);
		}
		
		map<string, User>::iterator it = users.find(name);
		if (it == users.end()) {
			cerr << "ERROR: User \"" << name << "\" not found in train set\n";
			exit(-1);
		}		
		
		Item item;
		bool legalName = false;
		for (Item i = Item(0); i < Item(ItemCount); i = Item(i+1)) {
			if (!movie.compare(Data::ItemName(i))) {
				item = i;
				legalName = true;
			}
		}
		if (!legalName) {
			cerr << "ERROR: Can't recognize movie: " << movie << endl;
			exit(-1);
		}
		it->second.ratings[item].score = rating;
		it->second.ratings[item].type = Validation;
		
		fval >> name >> movie >> rating;		
	}
	
	currentUser = users.begin();
}

void Data::Print() {
	cout << ASCII_RED << "============================ ";
	cout << "Data Set (Validation set colored)";
	cout << " ============================\n" << ASCII_RESET;
	cout << ASCII_YELLOW << "              ";
	for (Item i = Item(0); i < Item(ItemCount); i = Item(i+1)) {
		cout << Data::ItemName(i);
		cout << "  ";
	}
	cout << ASCII_RESET << endl;
	for (map<string, User>::iterator it = users.begin(); it != users.end(); it++) {
		User u = it->second;

		cout << ASCII_GREEN;
		cout.width(12);
		cout << u.name << ASCII_RESET;
		
		for (Item i = Item(0); i < Item(ItemCount); i = Item(i+1)) {
			Rating r = u.ratings[i];
			if (r.score < 0) {
				cout.width(Data::ItemName(i).length() + 2);
				cout << "";
			} else if (r.type == Training) {
				cout.width(Data::ItemName(i).length() + 2);
				cout << r.score;
			} else {
				cout << ASCII_PURPLE;
				cout.width(Data::ItemName(i).length() + 2);
				cout << r.score;
				cout << ASCII_RESET;
			}
		}
		cout << endl;
	}
}

double Data::getUserMean(User u) {
	double result = 0;
	int count = 0;
	for (Item i = Item(0); i != Item(ItemCount); i = Item(i+1)) {
		if (u.ratings[i].score == UNKNOWN || u.ratings[i].type == Validation) {
			continue;
		}

		result += u.ratings[i].score;
		count++;
	}
	
	return count > 0 ? result / count : 0.0;
}

double Data::getItemMean(Item i) {
	double result = 0;
	int count = 0;
	for (map<string, User>::iterator it = users.begin(); it != users.end(); it++) {
		User u = it->second;
		if (u.ratings[i].score == UNKNOWN || u.ratings[i].type == Validation) {
			continue;
		}
		
		result += u.ratings[i].score;
		count++;
	}
	
	return count > 0 ? result / count : 0.0;
}

User *Data::getNextUser() {
	if (currentUser == users.end()) {
		// No more users. Reset and return NULL.
		currentUser = users.begin();
		return NULL;
	}
	
	User *u = &(currentUser->second);
	currentUser++;
	return u;
}
