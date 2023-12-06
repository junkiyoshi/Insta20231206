#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofEnableDepthTest();

	auto radius = 20;
	auto x_span = radius * sqrt(3);
	auto flg = true;
	for (float y = -330; y < 330; y += radius * 1.5) {

		for (float x = -330; x < 330; x += x_span) {

			for (float z = -150; z <= 150; z += 5) {

				glm::vec3 location;
				if (flg) {

					location = glm::vec3(x, y, z);
				}
				else {

					location = glm::vec3(x + (x_span / 2), y, z);
				}

				this->location_list.push_back(location);
			}
		}
		flg = !flg;
	}

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	for (int i = 0; i < this->location_list.size(); i++) {

		this->setHexagonToMesh(this->face, this->frame, this->location_list[i], 20, 5);
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	this->face.draw();
	this->frame.draw();

	this->cam.end();

	/*
	int start = 150;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
void ofApp::setHexagonToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float height) {

	ofColor face_color = ofColor(ofMap(location.z, -150, 150, 0, 255));
	ofColor frame_color = ofColor(0);

	auto noise_param = ofNoise(location.x * 0.015, location.y * 0.015, location.z * 0.015, ofGetFrameNum() * 0.005);
	auto face_radius = radius - 0.5;
	if (noise_param < 0.45) {

		return;
	}
	else if (noise_param > 0.45 && noise_param < 0.5) {

		face_radius *= ofMap(noise_param, 0.45, 0.5, 0, 1);
	}

	for (int deg = 90; deg < 450; deg += 60) {
		
		auto face_index = face_target.getNumVertices();
		auto frame_index = frame_target.getNumVertices();

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(0, 0, 0));
		vertices.push_back(glm::vec3(face_radius * cos(deg * DEG_TO_RAD), face_radius * sin(deg * DEG_TO_RAD), 0));
		vertices.push_back(glm::vec3(face_radius * cos((deg + 60) * DEG_TO_RAD), face_radius * sin((deg + 60) * DEG_TO_RAD), 0));
		vertices.push_back(glm::vec3(0, 0, height));
		vertices.push_back(glm::vec3(face_radius * cos((deg + 60) * DEG_TO_RAD), face_radius * sin((deg + 60) * DEG_TO_RAD), height));
		vertices.push_back(glm::vec3(face_radius * cos(deg * DEG_TO_RAD), face_radius * sin(deg * DEG_TO_RAD), height));

		for (auto& vertex : vertices) {

			vertex += location;
		}

		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);
		face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 4);
		face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);

		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 2);
		frame_target.addIndex(frame_index + 4); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 4);

		for (int i = 0; i < vertices.size(); i++) {

			face_target.addColor(face_color);
			frame_target.addColor(frame_color);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}