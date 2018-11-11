#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <boost/chrono.hpp> //timing
#include <boost/format.hpp> //string formatting
#include <SerialPort.h>

using namespace std;
using namespace boost;

void PrintHelp() {
	cerr << "SerialChipScriptApp usage:" << endl;

	cerr << "  -f : input file" << endl;
	cerr << "  -p : communication port (e.g. COM1)" << endl;
	cerr << "  -h : print this message" << endl;
}

int main(int argc, char **argv) {

	if (argc == 1) {
		PrintHelp();
		return 0;
	}

	std::string file_name = argv[1];
	std::string port_name = "COM6";

	for (int i = 2; i < argc; i++) {
		if ((strcmp(argv[i], "-p") == 0) && (i < (argc - 1)))
			port_name = argv[++i];
		else if (strcmp(argv[i], "-h") == 0) { PrintHelp(); }
	}

	SimpleSerial *port;

	std::string line;
	int dt, chip_id, chip_nr, address, data;
	vector<int> data_buffer;
	chrono::high_resolution_clock::time_point t_prev = chrono::high_resolution_clock::now(), t_now;

	ifstream file;
	file.exceptions(std::ifstream::failbit);

	try {
		file.open(file_name);
	}
	catch (const std::ifstream::failure&) {
		cerr << "Error while opening/reading file: " << file_name << endl;
		return 1;
	}

	try {
		port = new SimpleSerial(port_name, 9600);
	}
	catch (boost::system::system_error&) {
		cerr << "Could not open the specified port: " << port_name << endl;
		return 1;
	}

	file.exceptions(0);

	vector<int> chip_regs(0x20, 0x100);

	int prev_dt = 0;
	int value;

	string output;

	while (getline(file, line)) {
		std::istringstream iss(line);

		if (!(iss >> dec >> dt >> hex >> chip_id >> chip_nr >> address >> data))
			break;

		data_buffer.clear();

		while (iss >> value) {
			data_buffer.push_back(value);
		}
		
		prev_dt += dt;

		if (chip_regs[address] == data) {
//			continue;
		}

		chip_regs[address] = data;
		output = boost::str(boost::format(" %X %X %X %X") % chip_id % chip_nr % address % data);
		for (unsigned int i = 0; i < data_buffer.size(); i++) {
			output.append(boost::str(boost::format(" %X") % data_buffer[i]));
		}
		output.append("\n");

		while (chrono::duration_cast<boost::chrono::milliseconds>(chrono::high_resolution_clock::now() - t_prev).count() < prev_dt)
			;
		t_prev = chrono::high_resolution_clock::now();

		cout << prev_dt << output;
		port->writeString(output);//can be too fast for arduino
		Sleep(1);
		prev_dt = 0;
	}

	file.close();

	return 0;
}

