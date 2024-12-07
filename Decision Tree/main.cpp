#include <bits/stdc++.h>
using namespace std;

#define max_attr 6

int i=0;

class Car{
	private:
	int buying, maint, doors, persons, lug_boot, safety;
	int class_value;

	int set_buying(string buying){
		if(buying == "vhigh") return 3;
		else if(buying == "high") return 2;
		else if(buying == "med") return 1;
		else if(buying == "low") return 0;
		else return -1;
	}

	int set_doors(string doors){
		return stoi(doors.substr(0,1)) - 2;
	}

	int set_persons(string persons){
		if(persons == "more") return 2;
		else return stoi(persons) / 2 - 1;
	}


	int set_lug_boot(string lug_boot){
		if(lug_boot == "small") return 0;
		else if(lug_boot == "med") return 1;
		else if(lug_boot == "big" || lug_boot == "high") return 2;
		else return -1;
	}

	int set_safety(string safety){
		if(safety == "low") return 0;
		else if(safety == "med") return 1;
		else if(safety == "high") return 2;
		else return -1;
	}

	int set_class_value(string class_value){
		if(class_value == "unacc") return 0;
		else if(class_value == "acc") return 1;
		else if(class_value == "good") return 2;
		else if(class_value == "vgood") return 3;
		else return -1;
	}

	public:
	Car(){

	}

	Car(string buying, string maint,string doors,string persons,
		string lug_boot,string safety,string class_value){

		this->buying=set_buying(buying);
		this->maint=set_buying(maint);
		this->doors=set_doors(doors);
		this->persons=set_persons(persons);
		this->lug_boot=set_lug_boot(lug_boot);
		this->safety=set_safety(safety);
		this->class_value=set_class_value(class_value);
	}


	string get_class_name(int class_value){
		vector<string> v = {"", "unacc", "acc", "good", "vgood"};
		return v[class_value+1];
	}

	int get_class_value(){
		return class_value;
	}

	int get_buying(){
		return buying;
	}

	int get_maint(){
		return maint;
	}

	int get_doors(){
		return doors;
	}

	int get_persons(){
		return persons;
	}

	int get_lug_boot(){
		return lug_boot;
	}

	int get_safety(){
		return safety;
	}

	int get_value(int attr){
		switch(attr){
			case 0: 
				return get_buying();
				break;
			case 1: 
				return get_maint();
				break;
			case 2: 
				return get_doors();
				break;
			case 3: 
				return get_persons();
				break;
			case 4: 
				return get_lug_boot();
				break;
			case 5: 
				return get_safety();
				break;
			case 6: 
				return get_class_value();
				break;
		}

		return -1;
	}

};

class Node{
	private:
	int split_attribute;
	int answer;
	vector<Node*> childs;

	public:
	Node(int split, int answer = -1){
		split_attribute = split;
		this->answer = answer;
	}

	void addChild(Node *child){
		childs.push_back(child);
	}

	void setAnswer(int val){
		answer = val;
	}

	void print(int tab=0, int index=0){
		cout<<string(tab, '\t')<<index<<" -> "<<split_attribute<<" "<<answer<<endl;

		int idx = 0;
		for(auto i: childs){
			i->print(tab+1, idx++);
		}
	}

	~Node(){
		for(auto i: childs){
			delete i;
		}
	}

	friend class DecisionTree;
};

vector<int> getClassCount(vector<Car> &cars){
	vector<int> class_count(4, 0);

	for(auto &car: cars){
		class_count[car.get_class_value()]++;
	}

	return class_count;
}

class DecisionTree{
	private:
	Node *root;

	double calculate_entropy(vector<Car> &cars){
		vector<int> class_count = getClassCount(cars);
		return calculate_B(class_count);
	}

	double calculate_B(vector<int> &class_count){
		double total = 0;
		for(auto i: class_count){
			total += i;
		}

		double entropy = 0;
		for(auto i: class_count){
			if(i == 0) continue;
			double p = i / total;
			entropy -= p * log2(p);
		}

		return entropy;
	}


	double calculate_reminder(vector<Car> &cars,int split){
		int table[4][4] = {0};
		double sum_table[4] = {0};
		double total = cars.size();

		for(auto &car: cars){
			table[car.get_value(split)][car.get_class_value()]++;
			sum_table[car.get_value(split)]++;
		}

		double remainder = 0;
		for(int i=0; i<4;i++){
			double class_p = sum_table[i] / total;
			double B = 0;
			for(int j=0; j<4; j++){
				if(table[i][j] == 0){
					continue;
				}
				double p = table[i][j] / sum_table[i];
				B += p * log2(p);
			}
			remainder += class_p * -B;
		}
		return remainder;
	}


	double calculate_gini(vector<int> &class_count){
		// vector<int> class_count = getClassCount(cars);
		double total = 0;
		for(auto i: class_count){
			total += i;
		}

		double gini = 1.0;
		for(auto i: class_count){
			if(i == 0) continue;
			double p = i / total;
			gini -= p * p;
		}

		return gini;
	}


	double calculate_gini_reminder(vector<Car> &cars, int split){
		int table[4][4] = {0};
		double sum_table[4] = {0};
		double total = cars.size();

		for(auto &car: cars){
			table[car.get_value(split)][car.get_class_value()]++;
			sum_table[car.get_value(split)]++;
		}

		double gini = 0.0;
		for(int i=0; i<4; i++){
			double class_p = sum_table[i] / total;
			if(class_p == 0){
				continue;
			}

			vector<int> class_count(table[i], table[i] + 4);
			double gini_impurity = calculate_gini(class_count);
			gini += class_p * gini_impurity;
		}

		return gini;
	}

	int find_best_split_entropy(vector<Car> &cars,bool is_random_3){
		vector<int> attribute_used(max_attr, 0);

		vector<pair<double, int>> gains;
		int updated = 0;
		double ent = calculate_entropy(cars);
		double mx_gain = 0;
		double mx_idx = 0;

		for(int i=0;i<attribute_used.size();i++){
			if(!attribute_used[i]){
				updated = 1;
				double reminder = calculate_reminder(cars, i);	
				double gain = ent - reminder;
				gains.push_back({gain, i});
				if(gain > mx_gain){
					mx_gain = gain;
					mx_idx = i;
				}
			}

		}

		sort(gains.begin(), gains.end(), [](const auto& a, const auto& b) {
			return a.first > b.first;
		});



		if(is_random_3){

			// cout << gains[0].first << endl;

			int limit = min(3, (int)gains.size());
			// cout<<"limit: "<<limit<<endl;
			int idx = rand() % limit; 

			// cout<<"idx : "<<idx<<endl;

			// cout<<idx<<" "<<gains.size()<<endl;
			mx_idx = gains[idx].second;
			// cout << mx_idx << endl;
		}

		return updated == 0 ? -1 : mx_idx;
	}

	int find_best_split_gini(vector<Car>& cars,bool is_random_3) {
		vector<int> attribute_used(max_attr, 0);
		vector<pair<double, int>> gini_values;
		double min_gini_reminder = INT_MAX;
		int min_idx = -1;

		for (int i = 0; i < max_attr; ++i) {
			if (attribute_used[i]){
				continue;
			}
			
			double gini_reminder = calculate_gini_reminder(cars, i);
			gini_values.push_back({gini_reminder, i});
			if (gini_reminder < min_gini_reminder) {
				min_gini_reminder = gini_reminder;
				min_idx = i;
			}
		}

		sort(gini_values.begin(), gini_values.end(), [](const auto& a, const auto& b) {
			return a.first < b.first;
		});

		if (is_random_3) {

			// cout << gini_values.size() << endl;

			int limit = min(3, (int)gini_values.size());
			// cout<<"limit: "<<limit<<endl;
			int idx = rand() % limit;

			// cout<<idx<<" "<<gini_values.size()<<endl;
			min_idx = gini_values[idx].second;
		}

		return min_idx;
	}



	int find_if_single_class(vector<Car> &cars){
		vector<int> class_count = getClassCount(cars);
		for(int i=0;i<class_count.size();i++){
			if(class_count[i] == cars.size()){
				return i;
			}
		}
		return -1;
	}

	int find_majority(vector<Car> &cars){
		vector<int> class_count = getClassCount(cars);
		int mx_idx = max_element(class_count.begin(), class_count.end()) - class_count.begin();
		return mx_idx;
	}

	Node* train_helper(vector<Car> cars, int majority,bool is_gini, bool is_random_3){
		if(cars.size() == 0){
			return new Node(-1, majority);
		}

		int if_one_class = find_if_single_class(cars);
		if(if_one_class != -1){
			return new Node(-2, if_one_class);
		}

		

		int split_at;
		if(!is_gini){
			split_at = find_best_split_entropy(cars,is_random_3);
		}else{
			split_at = find_best_split_gini(cars,is_random_3);
			// cout<<is_random_3<<" 00"<<endl;
		}

		int majority_class = find_majority(cars);

		if(split_at == -1){
			return new Node(-3, majority_class);
		}

		Node *node = new Node(split_at);
		// cout<<node->split_attribute<<" 00"<<endl;

		for(int i=0;i<4;i++){
			vector<Car> childCars;

			for(auto &car: cars){
				if(car.get_value(split_at) == i){
					childCars.push_back(car);
				}
			}

			Node *child = train_helper(childCars, majority_class, is_gini,is_random_3);
			node->addChild(child);
		}

		return node;
	}


	public:
	DecisionTree(){
		root = nullptr;
	}

	void train(vector<Car> cars,bool is_gini,bool is_random_3){
		root = train_helper(cars, -1,is_gini,is_random_3);
		// cout<<is_random_3<<" 00"<<endl;
	}


	int test(Car &car){
		Node* cur = root;
		while(cur){
			int split = cur->split_attribute;
			if(split < 0){
				return cur->answer;
			}

			int attr_value = car.get_value(split);

			cur = cur->childs[attr_value];
		}

		return -1;
	}

	vector<int> test(vector<Car> &cars){
		vector<int> ans;

		for(auto &i: cars){
			ans.push_back(test(i));
		}

		return ans;

	}

	~DecisionTree(){
		if(root != nullptr){
			delete root;
		}
	}


};


void file_output(ofstream &myOutput,string method,string best,int size,int match,double accuracy){
	myOutput << method<<" --> "<<best<<endl;
	myOutput << "-----------------------------------------------------"<<endl;	
	myOutput << "Test samples  : " << size << endl;
	myOutput << "Total matched : " <<  match << endl;
	myOutput << "Accuracy      : " << accuracy << endl;     
	myOutput << "\n" << endl;
}


int main() {
	ifstream myInput;
	myInput.open("./car evaluation dataset/car.data");
	ofstream myOutput;
	ofstream table;
	table.open("./car evaluation dataset/table.data");
	myOutput.open("./car evaluation dataset/output.data");
	string line;

	vector<Car> cars;
	while(getline(myInput, line)){
		stringstream s(line);
		string word;
		vector<string> v;
		while(getline(s, word, ',')){
			v.push_back(word);
		}
		assert(v.size() == max_attr + 1);
		cars.emplace_back(Car(v[0], v[1], v[2], v[3], v[4], v[5], v[6]));
	}

	vector<string> v = {"unacc", "acc", "good", "vgood"};

	int experiment = 20;

	vector<double> accuracy_info_best(experiment, 0);
	vector<double> accuracy_giny_best(experiment, 0);
	vector<double> accuracy_info_random(experiment, 0);
	vector<double> accuracy_giny_random(experiment, 0);
	double avg_accuracy_info_best = 0;
	double avg_accuracy_giny_best = 0;
	double avg_accuracy_info_random = 0;
	double avg_accuracy_giny_random = 0;

	// cout << cars[0].get_buying() << endl;

	srand(time(0));
	for(int i=0; i<experiment; i++){
		vector<Car> training_set,testing_set;

		for(auto &car: cars){
			int r = rand() % 10;
			if(r>=8){
				testing_set.push_back(car);
			}
			else{
				training_set.push_back(car);
			}
		}


		vector<int> total_class = getClassCount(cars);
		vector<int> training_class = getClassCount(training_set);
		vector<int> testing_class = getClassCount(testing_set);

		myOutput<<"--------------------------------------------------------------------------------------------------"<<endl;
		myOutput<<"----------------------------------------Test "<<i<<"----------------------------------------------------"<<endl;
		myOutput<<"--------------------------------------------------------------------------------------------------"<<endl;
		myOutput<<endl;
		myOutput<<"class\ttotal\ttrain\t test\t%train"<<endl;

		for(int j=0; j<4; j++){
			myOutput<<setw(5)<<v[j]<<"\t"<<setw(5)<<total_class[j]<<"\t"<<setw(5)<<training_class[j]<<"\t"<<setw(5)<<testing_class[j]<<"\t"<<training_class[j]/(double)total_class[j]<<endl;
		}
		myOutput<<endl;
		myOutput<<endl;

		


		DecisionTree tree_info_best, tree_giny_best, tree_info_random, tree_giny_random;


		//-----------------------------------------------------------
		//-----------------------------------------------------------

		// Information gain --> Always select the best attribute

		tree_info_best.train(training_set,false,false);
		vector<int> ans_info_best = tree_info_best.test(testing_set);
		int match_info_best = 0;
		
		for(int j=0; j<ans_info_best.size(); j++){
			if(ans_info_best[j] == testing_set[j].get_class_value()){
				match_info_best++;
			}
		}

		accuracy_info_best[i] = match_info_best / (double)testing_set.size();
		file_output(myOutput,"Information gain","Always select the best attribute",ans_info_best.size(),match_info_best,accuracy_info_best[i]);
		avg_accuracy_info_best += accuracy_info_best[i];

		//-----------------------------------------------------------
		//-----------------------------------------------------------
		tree_info_random.train(training_set,false,true);
		
		vector<int> ans_info_random = tree_info_random.test(testing_set);

		int match_info_random = 0; 

		for(int j=0; j<ans_info_random.size(); j++){
			if(ans_info_random[j] == testing_set[j].get_class_value()){
				match_info_random++;
			}
		}

		accuracy_info_random[i] = match_info_random / (double)testing_set.size();
		file_output(myOutput,"Information gain","Select one randomly from the top three attributes",ans_info_random.size(),match_info_random,accuracy_info_random[i]);
		avg_accuracy_info_random += accuracy_info_random[i];

		//-----------------------------------------------------------
		//-----------------------------------------------------------
		tree_giny_best.train(training_set,true,false);

		vector<int> ans_giny_best = tree_giny_best.test(testing_set);

		int match_giny_best = 0;

		for(int j=0; j<ans_giny_best.size(); j++){
			if(ans_giny_best[j] == testing_set[j].get_class_value()){
				match_giny_best++;
			}
		}

		accuracy_giny_best[i] = match_giny_best / (double)testing_set.size();
		file_output(myOutput,"Gini impurity","Always select the best attribute",ans_giny_best.size(),match_giny_best,accuracy_giny_best[i]);
		avg_accuracy_giny_best += accuracy_giny_best[i];

		//-----------------------------------------------------------
		//-----------------------------------------------------------
		// cout<<"00"<<endl;
		tree_giny_random.train(training_set,true,true);

		vector<int> ans_giny_random = tree_giny_random.test(testing_set);

		int match_giny_random = 0;

		for(int j=0; j<ans_giny_random.size(); j++){
			if(ans_giny_random[j] == testing_set[j].get_class_value()){
				match_giny_random++;
			}
		}


		accuracy_giny_random[i] = match_giny_random / (double)testing_set.size();
		file_output(myOutput,"Gini impurity","Select one randomly from the top three attributes",ans_giny_random.size(),match_giny_random,accuracy_giny_random[i]);
		avg_accuracy_giny_random += accuracy_giny_random[i];

		// cout << training_set.size()/(double)cars.size() << " " << testing_set.size()/(double)cars.size() << endl;
	}


	double bi,bg,ri,rg;

	bi = avg_accuracy_info_best / experiment;
	bg = avg_accuracy_giny_best / experiment;
	ri = avg_accuracy_info_random / experiment;
	rg = avg_accuracy_giny_random / experiment;


	table<<" _________________________________________________________________________________________________"<<endl;
	table<<"|                                           |                                                     |"<<endl;
	table<<"|                                           |            Average accuracy over 20 runs            |"<<endl;
	table<<"|___________________________________________|_____________________________________________________|"<<endl;
	table<<"|                                           |                           |                         |"<<endl;
	table<<"|       Attribute selection strategy        |     Information gain      |      Gini impurity      |"<<endl;
	table<<"|___________________________________________|___________________________|_________________________|"<<endl;
	table<<"|                                           |                           |                         |"<<endl;
	table<<"|     Always select the best attribute      |           "<<setprecision(4)<<bi<<"          |          "<<setprecision(4)<<bg<<"         |"<<endl;
	table<<"|___________________________________________|___________________________|_________________________|"<<endl;
	table<<"|                                           |                           |                         |"<<endl;
	table<<"|   Select one randomly from the top three  |           "<<setprecision(4)<<ri<<"          |          "<<setprecision(4)<<rg<<"         |"<<endl;
	table<<"|                attributes                 |                           |                         |"<<endl;
	table<<"|___________________________________________|___________________________|_________________________|"<<endl;

	return 0;
}