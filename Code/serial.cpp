#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>
using namespace std;

double* split(string str, char c, int num_elements) {
	double * out = new double[num_elements];
	int index=0;
	string e="";
	for(int i=0;i<str.length();i++) {
		if(str[i]==',') {
			out[index] = atof(e.c_str());
			index=index+1;
			e="";
		}else{
			e=e+str[i];
		}
	} 
	out[index] = atof(e.c_str());
	return out;
}

int get_file_size(string filename) {
  	ifstream file(filename);
    string str; 
    int ** read_data;
    int num_elements=0;
    while (std::getline(file, str)) {
    	num_elements=num_elements+1;
        // cout<<str<<endl;
    }
    return num_elements;
}

double** read_file(string filename, int num_attributes) {
  	int num_elements = get_file_size(filename);
  	ifstream file(filename);
    string str; 
    double ** read_data = new double*[num_elements];
    for(int i=0;i<num_elements;i++) {
    	read_data[i] = new double[num_attributes];
    }
    int i=0;
    while (std::getline(file, str)) {
    	read_data[i] = split(str,',',num_attributes);
    	i=i+1;
        // cout<<str<<endl;
    }	
    return read_data;
}

double find_dist_bw_2_objects(double* object1, double* object2, int num_attributes) {
	double dist = 0.0;
	for(int i=0;i<num_attributes;i++) {
		dist = dist + (object2[i]-object1[i])*(object2[i]-object1[i]);
	}
	dist = sqrt(dist);
	return dist;
}

double* sum(double* object1,double* object2,int num_attributes) {
	double* _sum = new double[num_attributes];
	for(int i=0;i<num_attributes;i++) {
		_sum[i] = object1[i]+object2[i];
	}
	return _sum;
}

int equal_or_not(double** object1,double** object2,int num_clusters,int num_attributes) {
	int out = 1;
	for(int i=0;i<num_clusters;i++) {
		for(int j=0;j<num_attributes;j++) {
			if(object1[i][j] != object2[i][j]) {
				out = 0;
			}
		}
	}
	return out;
}

double** divide(double** center_in_clusters, int* num_elements_in_clusters, int num_clusters, int num_attributes) {
	for(int i=0;i<num_clusters;i++) {
		for(int j=0;j<num_attributes;j++) {
			center_in_clusters[i][j] = center_in_clusters[i][j]/num_elements_in_clusters[i];
		}
	}
	return center_in_clusters;
}

void print_arr_of_arr(double** arr, int len1, int len2) {
	for(int i=0;i<len1;i++) {
		for(int j=0;j<len2;j++) {
			cout<<arr[i][j]<<",";
		}
		cout<<endl;
	}
}

int main(int argc, char** argv) {
	/*Input*/
	int num_attributes = 4;
	int num_clusters = 3;
	string filename = "iris.txt";
	/*---*/
	int num_elements = get_file_size(filename); // total number of objects
	double** all_objects = read_file(filename,num_attributes); // all objects and their attributes
	int * num_elements_in_clusters = new int[num_clusters]; // In each cluster, number of objects 
	
	double** center_in_clusters = new double*[num_clusters];
	for(int i=0;i<num_clusters;i++){
		num_elements_in_clusters[i] = 1;
		center_in_clusters[i] = all_objects[i];
	}


	while(true) {
		// cout<<"--------\n";
		// print_arr_of_arr(center_in_clusters,num_clusters,num_attributes);
		// cout<<"--------\n";
		vector< vector<int> > objectindexes_in_clusters;
		for(int i = 0; i < num_clusters; i++){
			vector<int> vec;
	    	objectindexes_in_clusters.push_back(vec);
	   	}

		double** center_in_clusters_new = new double*[num_clusters];
		for(int j=0;j<num_clusters;j++) { //initialize center_in_clusters_new
			center_in_clusters_new[j] = new double[num_attributes];
			for(int k=0;k<num_attributes;k++) {
				center_in_clusters_new[j][k] = 0;
			}
			num_elements_in_clusters[j] = 0;
		}
		for(int j=0;j<num_elements;j++) {
			double * object = all_objects[j];
			int position_of_cluster = 0; // position of the cluster which has the min distace with this object
			double min_dist = -1;
			for(int k=0;k<num_clusters;k++) {
				double* center_in_cluster = center_in_clusters[k];
				double dist = find_dist_bw_2_objects(object,center_in_cluster,num_attributes);

				if (min_dist == -1) {
					min_dist = dist;
					position_of_cluster = k;
				} else if(dist < min_dist) {
					min_dist = dist;
					position_of_cluster = k;
				}
				// cout<<"		obj:"<<j<<",k:"<<k<<",dist:"<<dist<<",min_dist:"<<min_dist<<",pos:"<<position_of_cluster<<endl;
			}//end for num_clusters
			center_in_clusters_new[position_of_cluster] = sum(center_in_clusters_new[position_of_cluster],object,num_attributes);
			num_elements_in_clusters[position_of_cluster] = num_elements_in_clusters[position_of_cluster]+1;
			objectindexes_in_clusters[position_of_cluster].push_back(j);
		}//end for num_elements
		center_in_clusters_new=divide(center_in_clusters_new, num_elements_in_clusters, num_clusters, num_attributes);
		
			
		cout<<"--------\n";
		print_arr_of_arr(center_in_clusters,num_clusters,num_attributes);
		cout<<"ok\n";
		print_arr_of_arr(center_in_clusters_new,num_clusters,num_attributes);
		cout<<"ok. indexes of ojects in clusters:\n";
		for(int j=0;j<num_clusters;j++) {
			for(int k=0;k<objectindexes_in_clusters[j].size();k++) {
				cout<<objectindexes_in_clusters[j][k]<<",";
			}
			cout<<"-"<<endl;
		}
		cout<<"num_elements_in_clusters:"<<endl;
		for(int j=0;j<num_clusters;j++) {
			cout<<num_elements_in_clusters[j]<<",";
		}
		cout<<"\n--------\n";

		if(equal_or_not(center_in_clusters_new,center_in_clusters,num_clusters,num_attributes)==1) {
			// cout<<"HEREEEEEEE,"<<i<<endl;
			break;
		}
		
		center_in_clusters = center_in_clusters_new;	
	}//end while
}



