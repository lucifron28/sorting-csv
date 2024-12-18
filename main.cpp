#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>

struct Artist {
    std::string name;
    int followers;
    int views;
    int likes;
    int fee;
};

void merge(std::vector<Artist>& artists, int left, int mid, int right, const std::string& criterion) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<Artist> L(n1);
    std::vector<Artist> R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = artists[left + i];
    for (int i = 0; i < n2; ++i)
        R[i] = artists[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        bool condition = false;
        if (criterion == "followers")
            condition = L[i].followers > R[j].followers;
        else if (criterion == "views")
            condition = L[i].views > R[j].views;
        else if (criterion == "likes")
            condition = L[i].likes > R[j].likes;

        if (condition) {
            artists[k] = L[i];
            ++i;
        } else {
            artists[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        artists[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        artists[k] = R[j];
        ++j;
        ++k;
    }
}

void mergeSort(std::vector<Artist>& artists, int left, int right, const std::string& criterion) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(artists, left, mid, criterion);
        mergeSort(artists, mid + 1, right, criterion);

        merge(artists, left, mid, right, criterion);
    }
}

void calculateFees(std::vector<Artist>& artists) {
    for (auto& artist : artists) {
        artist.fee = (artist.followers * 0.1) + (artist.views * 0.05) + (artist.likes * 0.2);
    }
}

void printArtists(const std::vector<Artist>& artists) {
    std::cout << std::left << std::setw(20) << "Name"
              << std::setw(10) << "Followers"
              << std::setw(10) << "Views"
              << std::setw(10) << "Likes"
              << std::setw(10) << "Fee" << "\n";
    for (const auto& artist : artists) {
        std::cout << std::left << std::setw(20) << artist.name
                  << std::setw(10) << artist.followers
                  << std::setw(10) << artist.views
                  << std::setw(10) << artist.likes
                  << std::setw(10) << artist.fee << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <csv_file>\n";
        return 1;
    }

    std::string csvFile = argv[1];

    std::ifstream file(csvFile);
    if (!file.is_open()) {
        std::cerr << "Could not open the file " << csvFile << "\n";
        return 1;
    }

    std::vector<Artist> artists;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Artist artist;
        std::getline(ss, artist.name, ',');
        ss >> artist.followers;
        ss.ignore();
        ss >> artist.views;
        ss.ignore();
        ss >> artist.likes;
        ss.ignore();
        ss >> artist.fee;
        artists.push_back(artist);
    }

    file.close();

    calculateFees(artists);

    std::string criterion;
    std::cout << "Enter sorting criterion (followers, views, likes): ";
    std::cin >> criterion;

    mergeSort(artists, 0, artists.size() - 1, criterion);

    printArtists(artists);

    return 0;
}