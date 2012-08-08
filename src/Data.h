//
//  Data.h
//  MatrixFactorizationOrientation
//
//  Created by Jerry Ni on 12/7/25.
//  Copyright (c) 2012年 woptspencil@gmail.com. All rights reserved.
//

#ifndef MatrixFactorizationOrientation_Data_h
#define MatrixFactorizationOrientation_Data_h

#define UNKNOWN -1

#define ItemCount	8

#define ASCII_RED		"\x1b[1;31m"
#define ASCII_GREEN		"\x1b[1;32m"
#define ASCII_YELLOW	"\x1b[1;33m"
#define ASCII_BLUE		"\x1b[1;34m"
#define ASCII_PURPLE	"\x1b[1;35m"
#define ASCII_CYAN		"\x1b[1;36m"
#define ASCII_RESET		"\x1b[0m"

#include <string>
#include <map>

using namespace std;

enum Item {
	Titanic = 0,
	Inception = 1,
	Avengers = 2,
	TheDarkKnight = 3,
	Ted = 4,
	GodFather = 5,
	SpiderMan = 6,
	Matrix = 7,
};

enum RatingType {
	Training = 0, Validation = 1,
};

typedef struct {
	int user;
	int item;
	int score;
	RatingType type;
} Rating;

class User {
public:
	User(string theName, int num);
	
	string name;
	int u_id;
	Rating ratings[ItemCount];
};

class Data {
public:
	Data(const char *trainFile, const char *valFile);
	void Print();
	static string ItemName(Item item);
	
	int getUserCount() { return userCount; }
	int getItemCount() { return ItemCount; }
	double getGlobalMean() { return mu; }
	double getUserMean(User u);
	double getItemMean(Item i);
	User *getNextUser();
	
protected:
	map<string, User> users;
	int userCount;
	double mu;
	
	// Iterators for getNextUser()
	map<string, User>::iterator currentUser;
};

#endif
