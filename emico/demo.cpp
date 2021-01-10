#include <emico.h>
#include <iostream>

using std::cout;
using std::endl;

int main() {

	cout << "There are " << emico::assets.size() << " embedded assets." << endl;

	for (auto &asset : emico::assets) {
		
		cout << "\t" << asset.first << // Unique ID based on the original file path.
			" : " << asset.second.first << // The memory address of the file contents.
			" (" << asset.second.second << " bytes) < "; // Length in bytes of the content.
		
		for (int i = 0; i < 10 && i < asset.second.second; i++) {
			cout << static_cast<int>(reinterpret_cast<const char *>(asset.second.first)[i]) << " ";
		}

		cout << " ... >" << endl;
	}
}