#include "../basicPackages.h"
#include <iomanip>

float t; 	// thickness of stack as measured WITHOUT first bbeam facing tungsten (quartz-to-quartz).
float qt; 	// thickness of the last piece of quartz
float qb; 	// max height of tallest piece of quartz
float qs; 	// height of short end of all quartz pieces
float d; 	// length of quartz and tungsten pieces (246 for open geometry)
float m= 0.277513; 	// slope of mirror 4
float wt;	// tungsten thickness


int main() {

cout.precision(3);


	// ------------------- INPUT FREE PARAMETERS ----------------------------
	
		//cout << "Input quartz-to-quartz stack thickness: \n" << endl;
		//cin >> t;
		
		cout << "\nInput thickness of the last piece of quartz to be hit by the beam: \n" << endl;
		cin >> qt;
		
		cout << "\nInput max height of the tallest piece of quartz: \n" << endl;
		cin >> qb;
		
		cout << "\nInput height of short end of all quartz pieces: \n" << endl;
		cin >> qs;
		
		cout << "\nInput tungsten thickness: \n" << endl;
		
		cin >> wt;
		
		cout << "\nInput length of quartz and tungsten pieces (246 for open geometry): \n" << endl;
		cin >> d;
		
		cout << "\nCalculating dimensions. \n" << endl;
		
		t= 4*qt+3*wt;
		
		
		
	// ------------------- CALCULATE QSIM DIMENSIONS ----------------------------
		
		
		

	
		// Mirror 1:
		
		float p1x;	//x coordinate of center point of mirror 1
		float p1y;	//y coordinate of center point of mirror 1
		float p1z=0;	//z coordinate of center point of mirror 1
		float L1;	//Length of largest side of polygon
		float l1;	//Length of shortest side of polygon
		float h1;	//Height of polygon
		float theta1x;	//Critical x rotation of polygon
		float theta1y;	//Critical x rotation of polygon
		
		p1x= t/(2*(1-m));
		
		p1y= d/2;
		
		
		L1=t*(1+m)/(1-m);
		
		l1= t;
		
		h1= t/(1-m);
		
		theta1x= 0;
		
		theta1y= 90;
		
		// translate position to sucky coordinate system
		p1x = p1x + qb/2 + qt/4;
		p1z = p1z - (t-qt)/2;
		
		
		
		// Truncate all values to two decimals
		
		p1x = roundf(p1x * 100) / 100;
		p1y = roundf(p1y * 100) / 100;
		p1z = roundf(p1z * 100) / 100;
		L1 = roundf(L1 * 100) / 100;
		l1 = roundf(l1 * 100) / 100;
		h1 = roundf(h1 * 100) / 100;
		theta1x = roundf(theta1x * 100) / 100;
		theta1y = roundf(theta1y * 100) / 100;
		
		
		
		
		
		// Mirror 1_1:
		
		float p1_1x;	//x coordinate of center point of mirror 1
		float p1_1y;	//y coordinate of center point of mirror 1
		float p1_1z=0;	//z coordinate of center point of mirror 1
		float L1_1;	//Length of largest side of polygon
		float l1_1;	//Length of shortest side of polygon
		float h1_1;	//Height of polygon
		float theta1_1x;	//Critical x rotation of polygon
		float theta1_1y;	//Critical x rotation of polygon
		
		p1_1x= t/(2*(1-m));
		
		p1_1y= -d/2;
		
		L1_1=  L1;
		
		l1_1= l1;
		
		h1_1= h1;
		
		theta1_1x= 0;
		
		theta1_1y= 90;
		
		// translate position to sucky coordinate system
		p1_1x = p1_1x + qb/2 + qt/4;
		p1_1z = p1_1z - (t-qt)/2;
		
		
		
		
		// Truncate all values to two decimals
		
		p1_1x = roundf(p1_1x * 100) / 100;
		p1_1y = roundf(p1_1y * 100) / 100;
		p1_1z = roundf(p1_1z * 100) / 100;
		L1_1 = roundf(L1_1 * 100) / 100;
		l1_1 = roundf(l1_1 * 100) / 100;
		h1_1 = roundf(h1_1 * 100) / 100;
		theta1_1x = roundf(theta1_1x * 100) / 100;
		theta1_1y = roundf(theta1_1y * 100) / 100;
		
		
		

		
		
		// Mirror 2:
		
		float p2x;	//x coordinate of center point of mirror 1
		float p2y;	//y coordinate of center point of mirror 1
		float p2z=0;	//z coordinate of center point of mirror 1
		float L2;	//Length of largest side of polygon
		float l2;	//Length of shortest side of polygon
		float h2;	//Height of polygon
		float theta2x;	//Critical x rotation of polygon
		float theta2y;	//Critical x rotation of polygon
		
		p2x= 125+t/(2*(1-m));
		
		p2y= (d+68)/4;
		
		L2= L1;
		
		l2= 70;
		
		h2= sqrt((250-t/(1-m))*(250-t/(1-m))+(d/2-34)*(d/2-34)); 
		
		theta2x=atan((d/2-34)/(250-t/(1-m)))*180/3.1415926;
		
		theta2y= 90;
		
		// translate position to sucky coordinate system
		p2x = p2x + qb/2 + qt/4;
		p2z = p2z - (t-qt)/2;
		
		
		// Truncate all values to two decimals
		
		p2x = roundf(p2x * 100) / 100;
		p2y = roundf(p2y * 100) / 100;
		p2z = roundf(p2z * 100) / 100;
		L2 = roundf(L2 * 100) / 100;
		l2 = roundf(l2 * 100) / 100;
		h2 = roundf(h2 * 100) / 100;
		theta2x = roundf(theta2x * 100) / 100;
		theta2y = roundf(theta2y * 100) / 100;
		
		
		
		
		
		
		
		
		
		// Mirror 2_1:
		
		float p2_1x;	//x coordinate of center point of mirror 1
		float p2_1y;	//y coordinate of center point of mirror 1
		float p2_1z=0;	//z coordinate of center point of mirror 1
		float L2_1;	//Length of largest side of polygon
		float l2_1;	//Length of shortest side of polygon
		float h2_1;	//Height of polygon
		float theta2_1x;	//Critical x rotation of polygon
		float theta2_1y;	//Critical x rotation of polygon
		
		p2_1x= 125+t/(2*(1-m));
		
		p2_1y= -((d+68)/4);
		
		L2_1= L2;
		
		l2_1= l2;
		
		h2_1= h2;
		
		theta2_1x= -theta2x;
		
		theta2_1y= 90;
		
		// translate position to sucky coordinate system
		p2_1x = p2_1x + qb/2 + qt/4;
		p2_1z = p2_1z - (t-qt)/2;
		
		
		
		// Truncate all values to two decimals
		
		p2_1x = roundf(p2_1x * 100) / 100;
		p2_1y = roundf(p2_1y * 100) / 100;
		p2_1z = roundf(p2_1z * 100) / 100;
		L2_1 = roundf(L2_1 * 100) / 100;
		l2_1 = roundf(l2_1 * 100) / 100;
		h2_1 = roundf(h2_1 * 100) / 100;
		theta2_1x = roundf(theta2_1x * 100) / 100;
		theta2_1y = roundf(theta2_1y * 100) / 100;
		
		
		
		
		
		
		
		
		
		// Mirror 4:
		
		float p4x;	//x coordinate of center point of mirror 1
		float p4y=0;	//y coordinate of center point of mirror 1
		float p4z;	//z coordinate of center point of mirror 1
		float L4;	//Length of largest side of polygon
		float l4;	//Length of shortest side of polygon
		float h4;	//Height of polygon
		float theta4x;	//Critical x rotation of polygon
		float theta4z;	//Critical x rotation of polygon
		
		p4x= t/(2*(1-m));
		
		p4z= t/(2*(1-m));
		
		L4= d;
		
		l4= L4;
		
		h4= sqrt(t*t/((1-m)*(1-m))+((1+m)*t/(2-2*m)-t/2)*((1+m)*t/(2-2*m)-t/2));
		
		theta4x= -90-15.51;
		
		theta4z= 90;
		
		
		// translate position to sucky coordinate system
		p4x = p4x + qb/2 + qt/4;
		p4z = p4z - (t-qt)/2;
		
		
		// Truncate all values to two decimals
		
		p4x = roundf(p4x * 100) / 100;
		p4y = roundf(p4y * 100) / 100;
		p4z = roundf(p4z * 100) / 100;
		L4 = roundf(L4 * 100) / 100;
		l4 = roundf(l4 * 100) / 100;
		h4 = roundf(h4 * 100) / 100;
		theta4x = roundf(theta4x * 100) / 100;
		theta4z = roundf(theta4z * 100) / 100;
		
		
		
		
		
		
		// Mirror 4_1:
		
		float p4_1x;	//x coordinate of center point of mirror 1
		float p4_1y=0;	//y coordinate of center point of mirror 1
		float p4_1z;	//z coordinate of center point of mirror 1
		float L4_1;	//Length of largest side of polygon
		float l4_1;	//Length of shortest side of polygon
		float h4_1;	//Height of polygon
		float theta4_1x;	//Critical x rotation of polygon
		float theta4_1z;	//Critical x rotation of polygon
		
		p4_1x= t/(2*(1-m));
		
		p4_1z= -t/(2*(1-m));
		
		L4_1= L4;
		
		l4_1= l4;
		
		h4_1= h4;
		
		theta4_1x=  -90+15.51;
		
		theta4_1z= 90;
		
		
		// translate position to sucky coordinate system
		p4_1x = p4_1x + qb/2 + qt/4;
		p4_1z = p4_1z - (t-qt)/2;
	
		
		
		// Truncate all values to two decimals
		
		p4_1x = roundf(p4_1x * 100) / 100;
		p4_1y = roundf(p4_1y * 100) / 100;
		p4_1z = roundf(p4_1z * 100) / 100;
		L4_1 = roundf(L4_1 * 100) / 100;
		l4_1 = roundf(l4_1 * 100) / 100;
		h4_1 = roundf(h4_1 * 100) / 100;
		theta4_1x = roundf(theta4_1x * 100) / 100;
		theta4_1z = roundf(theta4_1z * 100) / 100;
		
		
		
		
		
		
		// Mirror 6:
		
		float p6x;	//x coordinate of center point of mirror 1
		float p6y=0;	//y coordinate of center point of mirror 1
		float p6z;	//z coordinate of center point of mirror 1
		float L6;	//Length of largest side of polygon
		float l6;	//Length of shortest side of polygon
		float h6;	//Height of polygon
		float theta6x;	//Critical x rotation of polygon
		float theta6z;	//Critical x rotation of polygon
		
		p6x= t/(2*(1-m))+125;
		
		p6z=17.5 + (t/4)*(1+m)/(1-m);
		
		L6 = d;
		
		l6=68;
		
		h6=sqrt((250-t/(1-m))*(250-t/(1-m))+((1+m)*t/(2-2*m)-35)*((1+m)*t/(2-2*m)-35));
		
		theta6x = -90+atan(((1+m)*t/(2-2*m)-35)/(250-t/(1-m)))*180/3.14159265;
		
		theta6z= 90;
		
		
		// translate position to sucky coordinate system
		p6x = p6x + qb/2 + qt/4;
		p6z = p6z - (t-qt)/2;
		
		
		
		// Truncate all values to two decimals
		
		p6x = roundf(p6x * 100) / 100;
		p6y = roundf(p6y * 100) / 100;
		p6z = roundf(p6z * 100) / 100;
		L6 = roundf(L6 * 100) / 100;
		l6 = roundf(l6 * 100) / 100;
		h6 = roundf(h6 * 100) / 100;
		theta6x = roundf(theta6x * 100) / 100;
		theta6z = roundf(theta6z * 100) / 100;
		
		
		
			
		
		
		// Mirror 6_1:
		
		float p6_1x;	//x coordinate of center point of mirror 1
		float p6_1y=0;	//y coordinate of center point of mirror 1
		float p6_1z;	//z coordinate of center point of mirror 1
		float L6_1;	//Length of largest side of polygon
		float l6_1;	//Length of shortest side of polygon
		float h6_1;	//Height of polygon
		float theta6_1x;	//Critical x rotation of polygon
		float theta6_1z;	//Critical x rotation of polygon
		
		p6_1x= t/(2*(1-m))+125;
		
		p6_1z= -(17.5 + (t/4)*(1+m)/(1-m));
		
		L6_1= L6;
		
		l6_1= l6;
		
		h6_1= h6;
		
		theta6_1x = -90-atan(((1+m)*t/(2-2*m)-35)/(250-t/(1-m)))*180/3.14159265;
		
		theta6_1z= 90;
		
		
		// translate position to sucky coordinate system
		p6_1x = p6_1x + qb/2 + qt/4;
		p6_1z = p6_1z - (t-qt)/2;
		
		
		// Truncate all values to two decimals
		p6_1x = roundf(p6_1x * 100) / 100;
		p6_1y = roundf(p6_1y * 100) / 100;
		p6_1z = roundf(p6_1z * 100) / 100;
		L6_1 = roundf(L6_1 * 100) / 100;
		l6_1 = roundf(l6_1 * 100) / 100;
		h6_1 = roundf(h6_1 * 100) / 100;
		theta6_1x = roundf(theta6_1x * 100) / 100;
		theta6_1z = roundf(theta6_1z * 100) / 100;
		
		
		
		
		
	

	
	
	
	
	
		
	
	
	
	
	
	
	
	
	// ------------------- WRITE PARAMETERS TO TEXT FILE ----------------------------
	
	
	
	
		stringstream stream; // Declares a stream to be saved as string
		stream << "mirrorTranslations.txt"; // Saves text into a string
	
		string filename = stream.str(); // declares previous string to be called filename
		
		
		ofstream myfile(filename.c_str()); // opens file which has the name of the string saved in filename
		
		
		if (myfile.is_open()){
		
			// QUARTZ Dimensions.
			
			// The following replace lines 90-93 in qsimDetectorConstruction.cc
			
			
			myfile << "        quartz_x = "<< d << "*mm/2; // replaces lines 90-93" << endl;
			myfile << "        quartz_y = "<< qb << "*mm/2; // 117.5 (open) ---> 152.5 (close)" << endl;
			myfile << "        //Change quartz thickness here." << endl;
			myfile << "        quartz_z = "<< qt << "*mm/2; // REPLACE END" << endl;
			
			
			//Tungsten Thickness
			myfile << "         Wthickness = " << wt << "*mm/2-0.01*mm/2; // REPLACES LINE 374 (this is new)" << endl;
			
			// Tungsten placement
			
			// The following replace lines 390-400
			
			
			myfile << "         G4VPhysicalVolume* tungsten_box_phys_1 // Replaces lines 390-400" << endl;
			
			myfile << "         = new G4PVPlacement(rotTung,G4ThreeVector(-" << qt << "*mm/4,0,quartz_zPos-quartz_z-" << wt << "*mm/2),tungsten_box_log,\"tungsten\", det_log,false,0); " << endl << endl;
			
			myfile << "         G4VPhysicalVolume* tungsten_box_phys_2" << endl;
			
			myfile << "         = new G4PVPlacement(rotTung,G4ThreeVector(-" << qt << "*mm/4,0,quartz_zPos-3*quartz_z-3*" << wt << "*mm/2),tungsten_box_log,\"tungsten\", det_log,false,0); " << endl << endl;
			
			myfile << "         G4VPhysicalVolume* tungsten_box_phys_3" << endl;
			
			myfile << "         = new G4PVPlacement(rotTung,G4ThreeVector(-" << qt << "*mm/4,0,quartz_zPos-5*quartz_z-5*" << wt << "*mm/2),tungsten_box_log,\"tungsten\", det_log,false,0);" << endl << endl;
			
			myfile << "         G4VPhysicalVolume* tungsten_box_phys_4" << endl;
			
			myfile << "         = new G4PVPlacement(rotTung,G4ThreeVector(-" << qt << "*mm/4,0,quartz_zPos-7*quartz_z-7*" << wt << "*mm/2-0.5*mm),tungsten_box_log,\"tungsten\", det_log,false,0); // REPLACE END" << endl;
			
			// Quart Placement
			
			// The following replaces lines 408-425
			
			
			myfile << "         G4VPhysicalVolume* quartz_phys_c1 // REPLACES lines 408-425 (THIS IS NEW)" << endl;
			myfile << "         = new G4PVPlacement(rotQ_c1,G4ThreeVector(0,0,quartz_zPos-2*quartz_z-" << wt << "*mm),quartz_log,\"Quartz\", det_log,false,0);" << endl << endl;
			
			myfile << "         G4RotationMatrix* rotQ_c2 = new G4RotationMatrix;" << endl;
			myfile << "         rotQ_c2->rotateX(M_PI/2.*rad);" << endl;
			myfile << "         rotQ_c2->rotateZ(0.*deg);" << endl;
			myfile << "         //rotQ_c1->rotateX(180.*deg);" << endl << endl;
			
			myfile << "         G4VPhysicalVolume* quartz_phys_c2" << endl;
			myfile << "         = new G4PVPlacement(rotQ_c2,G4ThreeVector(0,0,quartz_zPos-4*quartz_z-2*" << wt << "*mm),quartz_log,\"Quartz\", det_log,false,0);" << endl << endl;
			
			myfile << "         G4RotationMatrix* rotQ_c3 = new G4RotationMatrix;" << endl;
			myfile << "         rotQ_c3->rotateX(M_PI/2.*rad);" << endl;
			myfile << "         rotQ_c3->rotateZ(0.*deg);" << endl;
			myfile << "         rotQ_c3->rotateX(180.*deg);" << endl << endl;
			
			myfile << "         G4VPhysicalVolume* quartz_phys_c3" << endl;
			myfile << "         = new G4PVPlacement(rotQ_c3,G4ThreeVector(0,0,quartz_zPos-6*quartz_z-3*" << wt << "*mm),quartz_log,\"Quartz\", det_log,false,0); // REPLACE END" << endl;
			
			
		
			// Mirror Dimensions
			
			// The following lines need to replace lines 540-570 
			
			myfile << "        G4Trap* mirror_box_1 = new G4Trap(\"mirror_box_1\"," << L1 << "*mm/2," << l1 << "*mm/2,0.5*mm/2,0.5*mm/2," << h1 << "*mm/2); // Replaces lines 540-570" << endl << endl;
			
			myfile << "        G4LogicalVolume* mirror_box_1_log = new G4LogicalVolume(mirror_box_1,Mirror,\"mirror_box_1_log\",0,0,0);" << endl << endl;
			
			myfile << "        G4VisAttributes *mirror_boxx_1 = new G4VisAttributes();" << endl;
			myfile << "        mirror_boxx_1->SetForceWireframe(true);" << endl;
			myfile << "        mirror_box_1_log->SetVisAttributes(mirror_boxx_1);" << endl << endl;
			
			myfile << "        G4Trap* mirror_box_2 = new G4Trap(\"mirror_box_2\"," << l2 << "*mm/2," << L2 << "*mm/2,0.5*mm/2,0.5*mm/2," << h2 << "*mm/2);" << endl << endl;
			
			myfile << "        G4LogicalVolume* mirror_box_2_log = new G4LogicalVolume(mirror_box_2,Mirror,\"mirror_box_1_1_log\",0,0,0);" << endl << endl;
			
			myfile << "        G4VisAttributes *mirror_boxx_2 = new G4VisAttributes();" << endl;
			myfile << "        mirror_boxx_2->SetForceWireframe(true);" << endl;
			myfile << "        mirror_box_2_log->SetVisAttributes(mirror_boxx_2);" << endl << endl;
			
			
			myfile << "        G4Trap* mirror_box_4 = new G4Trap(\"mirror_box_4\"," << L4 << "*mm/2," << l4 << "*mm/2,0.5*mm/2,0.5*mm/2," << h4 << "*mm/2);" << endl << endl;
			
			myfile << "         G4LogicalVolume* mirror_box_4_log = new G4LogicalVolume(mirror_box_4,Mirror,\"mirror_box_4_log\",0,0,0);" << endl << endl;
			
			myfile << "        G4VisAttributes *mirror_boxx_4 = new G4VisAttributes();" << endl;
			myfile << "        mirror_boxx_4->SetForceWireframe(true);" << endl;
			myfile << "        mirror_box_4_log->SetVisAttributes(mirror_boxx_4);" << endl << endl;
			
			myfile << "        G4Trap* mirror_box_6 = new G4Trap(\"mirror_box_6\"," << l6 << "*mm/2," << L6 << "*mm/2,0.5*mm/2,0.5*mm/2," << h6 << "*mm/2);" << endl << endl;
			
			myfile << "        G4LogicalVolume* mirror_box_6_log = new G4LogicalVolume(mirror_box_6,Mirror,\"mirror_box_6_log\",0,0,0);" << endl << endl;
			
			myfile << "        G4VisAttributes *mirror_boxx_6 = new G4VisAttributes();" << endl;
			myfile << "        mirror_boxx_6->SetForceWireframe(true);" << endl;
			myfile << "        mirror_box_6_log->SetVisAttributes(mirror_boxx_6); // REPLACE END" << endl;
			
			
			
			// Suitcase dimensions
			
			// Replaces lines 596-614
			
			myfile << "        G4Box* mirror_box_7 = new G4Box(\"mirror_box_5\"," << qb << "*mm/2,0.5*mm/2," << t << "*mm/2+1.0*mm); // Replaces lines 596-614 " << endl << endl;
			
			myfile << "        G4LogicalVolume* mirror_box_7_log" << endl;
			myfile << "        = new G4LogicalVolume(mirror_box_7,Mirror,\"mirror_box_7_log\",0,0,0);" << endl << endl;
			
			myfile << "        G4VisAttributes *mirror_boxx_7 = new G4VisAttributes();" << endl;
			myfile << "        mirror_boxx_7->SetForceWireframe(true);" << endl;
			myfile << "        mirror_box_7_log->SetVisAttributes(mirror_boxx_7);" << endl << endl;
			
			myfile << "        G4Box* mirror_box_8 = new G4Box(\"mirror_box_8\"," << qb << "*mm/2," << d << "*mm/2,0.5*mm/2); " << endl << endl; // CARLOS HAD 245.2 in place of d. WHY?? Why not 246??
			
			myfile << "        G4LogicalVolume* mirror_box_8_log" << endl;
			myfile << "        = new G4LogicalVolume(mirror_box_8,Mirror,\"mirror_box_8_log\",0,0,0);" << endl << endl;
			
			myfile << "        G4VisAttributes *mirror_boxx_8 = new G4VisAttributes();" << endl;
			myfile << "        mirror_boxx_8->SetForceWireframe(true);" << endl;
			myfile << "        mirror_box_8_log->SetVisAttributes(mirror_boxx_8);" << endl << endl;
			
			myfile << "        G4Box* mirror_box_9 = new G4Box(\"mirror_box_6\",0.5*mm/2," << d << "*mm/2," << t << "*mm/2); // REPLACE END" << endl;
		
		
		
			// Mirror 1
			
			//The following lines need to replace lines 860-938 in qsimDetectorConstruction.cc
			
			
			
			myfile << "        G4RotationMatrix* rotM_1 = new G4RotationMatrix; // Replaces lines 860-938 " <<  endl; // writes the first line of the file
			myfile << "        rotM_1->rotateY(" << theta1y << "*deg);" << endl; // writes the second name of the file
			myfile << "        rotM_1->rotateX(" << theta1x << "*deg); \n" << endl;
			
			myfile << "        G4ThreeVector zTrans_1(" << p1x << "*mm," << p1y << "*mm," << p1z << "*mm);\n" << endl;
			
			myfile << "        G4VPhysicalVolume* mirror_box_1_phys" << endl;
			myfile << "        = new G4PVPlacement(rotM_1,zTrans_1,mirror_box_1_log,\"mirror_box_1_phys\",det_log,false,0);\n" << endl << endl << endl;
			
			// Mirror 1_1
			myfile << "        G4RotationMatrix* rotM_1_1 = new G4RotationMatrix;" <<  endl; 
			myfile << "        rotM_1_1->rotateY(" << theta1_1y << "*deg);" << endl; 
			myfile << "        rotM_1_1->rotateX(" << theta1_1x << "*deg); \n" << endl;
			
			myfile << "        G4ThreeVector zTrans_1_1(" << p1_1x << "*mm," << p1_1y << "*mm," << p1_1z << "*mm);\n" << endl;
			
			myfile << "        G4VPhysicalVolume* mirror_box_1_1_phys" << endl;
			myfile << "        = new G4PVPlacement(rotM_1_1,zTrans_1_1,mirror_box_1_log,\"mirror_box_1_1_phys\",det_log,false,0);" << endl << endl << endl;
			
			
			// Mirror 2
			myfile << "        G4RotationMatrix* rotM_2 = new G4RotationMatrix;" <<  endl; 
			myfile << "        rotM_2->rotateY(" << theta2y << "*deg);" << endl; 
			myfile << "        rotM_2->rotateX(" << theta2x << "*deg); \n" << endl;
			
			myfile << "        G4ThreeVector zTrans_2(" << p2x << "*mm," << p2y << "*mm," << p2z << "*mm);\n" << endl;
			
			myfile << "        G4VPhysicalVolume* mirror_box_2_phys" << endl;
			myfile << "        = new G4PVPlacement(rotM_2,zTrans_2,mirror_box_2_log,\"mirror_box_2_phys\",det_log,false,0);" << endl << endl << endl;
			
			
			// Mirror 2_1
			myfile << "        G4RotationMatrix* rotM_2_1 = new G4RotationMatrix;" <<  endl; 
			myfile << "        rotM_2_1->rotateY(" << theta2_1y << "*deg);" << endl; 
			myfile << "        rotM_2_1->rotateX(" << theta2_1x << "*deg); \n" << endl;
			
			myfile << "        G4ThreeVector zTrans_2_1(" << p2_1x << "*mm," << p2_1y << "*mm," << p2_1z << "*mm);\n" << endl;
			
			myfile << "        G4VPhysicalVolume* mirror_box_2_1_phys" << endl;
			myfile << "        = new G4PVPlacement(rotM_2_1,zTrans_2_1,mirror_box_2_log,\"mirror_box_2_1_phys\",det_log,false,0);" << endl << endl << endl;
			
			// Mirror 4
			myfile << "        G4RotationMatrix* rotM_4 = new G4RotationMatrix;" <<  endl; 
			myfile << "        rotM_4->rotateZ(" << theta4z << "*deg);" << endl; 
			myfile << "        rotM_4->rotateX(" << theta4x << "*deg); \n" << endl;
			
			myfile << "        G4ThreeVector zTrans_4(" << p4x << "*mm," << p4y << "*mm," << p4z << "*mm);\n" << endl;
			
			myfile << "        G4VPhysicalVolume* mirror_box_4_phys" << endl;
			myfile << "        = new G4PVPlacement(rotM_4,zTrans_4,mirror_box_4_log,\"mirror_box_4_phys\",det_log,false,0);" << endl << endl << endl;
			
			
			
			// Mirror 4_1
			myfile << "        G4RotationMatrix* rotM_4_1 = new G4RotationMatrix;" <<  endl; 
			myfile << "        rotM_4_1->rotateZ(" << theta4_1z << "*deg);" << endl; 
			myfile << "        rotM_4_1->rotateX(" << theta4_1x << "*deg); \n" << endl;
			
			myfile << "        G4ThreeVector zTrans_4_1(" << p4_1x << "*mm," << p4_1y << "*mm," << p4_1z << "*mm);\n" << endl;
			
			myfile << "        G4VPhysicalVolume* mirror_box_4_1_phys" << endl;
			myfile << "        = new G4PVPlacement(rotM_4_1,zTrans_4_1,mirror_box_4_log,\"mirror_box_4_1_phys\",det_log,false,0);" << endl << endl << endl;
			
			
			// Mirror 6
			myfile << "        G4RotationMatrix* rotM_6 = new G4RotationMatrix;" <<  endl; 
			myfile << "        rotM_6->rotateZ(" << theta6z << "*deg);" << endl; 
			myfile << "        rotM_6->rotateX(" << theta6x << "*deg); \n" << endl;
			
			myfile << "        G4ThreeVector zTrans_6(" << p6x << "*mm," << p6y << "*mm," << p6z << "*mm);\n" << endl;
			
			myfile << "        G4VPhysicalVolume* mirror_box_6_phys" << endl;
			myfile << "        = new G4PVPlacement(rotM_6,zTrans_6,mirror_box_6_log,\"mirror_box_6_phys\",det_log,false,0);" << endl << endl << endl;
			
			
			// Mirror 6_1
			myfile << "        G4RotationMatrix* rotM_6_1 = new G4RotationMatrix;" <<  endl; 
			myfile << "        rotM_6_1->rotateZ(" << theta6_1z << "*deg);" << endl; 
			myfile << "        rotM_6_1->rotateX(" << theta6_1x << "*deg); \n" << endl;
			
			myfile << "        G4ThreeVector zTrans_6_1(" << p6_1x << "*mm," << p6_1y << "*mm," << p6_1z << "*mm);" << endl << endl;
			
			myfile << "        G4VPhysicalVolume* mirror_box_6_1_phys" << endl;
			myfile << "        = new G4PVPlacement(rotM_6_1,zTrans_6_1,mirror_box_6_log,\"mirror_box_6_1_phys\",det_log,false,0); // REPLACE END" << endl;
			
			
			
			
			
			// Suitcase Translations
			
			// Replaces 993 -1034
			
			float wierdDimension = t/2-qt/2; // WHERE DOES THIS COME FROM CARLOS?? 28.36
			float drop = (qb - qt/2)/2; // downard shift of mirror 9
			float shift = (t-qt)/2; // z shift of mirror 9
			
			myfile << "        G4ThreeVector zTrans_7(" << qt/2 << "*mm/2," << d/2 << "*mm+0.5*mm/2+0.01*mm,-" << wierdDimension << "*mm); // Replaces lines 993-1034" << endl << endl;
			
			myfile << "         G4RotationMatrix* rotM_7 = new G4RotationMatrix;" << endl;
			myfile << "         rotM_7->rotateY(0.0*deg);" << endl;
			myfile << "         rotM_7->rotateZ(0.0*deg);" << endl << endl;
			
			myfile << "         G4VPhysicalVolume* mirror_box_7_phys" << endl;
			myfile << "         = new G4PVPlacement(rotM_7,zTrans_7,mirror_box_7_log,\"mirror_box_7_phys\"," << endl;
			myfile << "                          det_log,false,0);" << endl << endl << endl;
			
			myfile << "        G4ThreeVector zTrans_7_1(" << qt/2 << "*mm/2,-" << d/2 << "*mm-0.5*mm/2-0.01*mm,-" << wierdDimension << "*mm);" << endl << endl;
			
			myfile << "         G4RotationMatrix* rotM_7_1 = new G4RotationMatrix;" << endl;
			myfile << "         rotM_7_1->rotateY(0.0*deg);" << endl;
			myfile << "         rotM_7_1->rotateZ(0.0*deg);" << endl << endl;
			
			myfile << "         G4VPhysicalVolume* mirror_box_7_1_phys" << endl;
			myfile << "         = new G4PVPlacement(rotM_7_1,zTrans_7_1,mirror_box_7_log,\"mirror_box_7_1_phys\"," << endl;
			myfile << "                          det_log,false,0);" << endl << endl;
			
			myfile << "        G4ThreeVector zTrans_8(" << qt/2 << "*mm/2,0.0*mm," << qt << "*mm/2+0.5*mm/2+0.01*mm);" << endl << endl;
			
			myfile << "         G4RotationMatrix* rotM_8 = new G4RotationMatrix;" << endl;
			myfile << "         rotM_8->rotateY(0.0*deg);" << endl;
			myfile << "         rotM_8->rotateZ(0.0*deg);" << endl << endl;
			
			myfile << "         G4VPhysicalVolume* mirror_box_8_phys" << endl;
			myfile << "         = new G4PVPlacement(rotM_8,zTrans_8,mirror_box_8_log,\"mirror_box_8_phys\"," << endl;
			myfile << "                          det_log,false,0);" << endl << endl;
			
			myfile << "        G4ThreeVector zTrans_8_1(" << qt/2 << "*mm/2,0.0*mm,-7*" << qt << "*mm/2-3*" << wt << "*mm-0.5*mm/2-0.01*mm);" << endl << endl;
			
			myfile << "         G4RotationMatrix* rotM_8_1 = new G4RotationMatrix;" << endl;
			myfile << "         rotM_8_1->rotateY(0.0*deg);" << endl;
			myfile << "         rotM_8_1->rotateZ(0.0*deg);" << endl << endl;
			
			myfile << "         G4VPhysicalVolume* mirror_box_8_1_phys" << endl;
			myfile << "         = new G4PVPlacement(rotM_8_1,zTrans_8_1,mirror_box_8_log,\"mirror_box_8_1_phys\"," << endl;
			myfile << "                          det_log,false,0);" << endl << endl;
			
			myfile << "        G4ThreeVector zTrans_9(-" << drop << "*mm-0.5*mm/2-0.01*mm,0.0*mm,-" << shift << "*mm); // END REPLACE" << endl;







			// PMT POSITION
			
			float pmtHeight = 250+qb/2 + qt/4;
			float pmtZval = (t-qt)/2;
			
			myfile << "        G4VPhysicalVolume* pmt_phys = new G4PVPlacement(rot_pmt,G4ThreeVector(" << pmtHeight << "*mm+plngth,0.0*mm,-" << pmtZval << "*mm),pmt_log,\"PMT\",det_log,false,0); // Replaces lines 1046-1047 " << endl; 
			myfile << "        G4VPhysicalVolume* cath_phys = new G4PVPlacement(rot_pmt,G4ThreeVector(" << pmtHeight << "*mm+2*plngth,0.0*mm,-" << pmtZval << "*mm),cath_log,\"CATH\",det_log,false,0); // REPLACE END"; 



			myfile.close(); // closes the file
			
		
		}
		
		else cout << "unable to open mirrorTranslations.txt file";
		
		
	system("gedit mirrorTranslations.txt &");
	system("perl translateSHMX.pl > qsimDetectorConstructionDifferences.out");

	
		
	
	return 0;
	
}

