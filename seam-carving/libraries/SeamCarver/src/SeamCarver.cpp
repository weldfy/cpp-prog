#include "SeamCarver.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

SeamCarver::SeamCarver(Image image) : m_image(std::move(image)) {}

const Image &SeamCarver::GetImage() const {
    return m_image;
}

size_t SeamCarver::GetImageWidth() const {
    return m_image.m_table.size();
}

size_t SeamCarver::GetImageHeight() const {
    return m_image.m_table[0].size();
}

double GetEnergy(Image::Pixel first, Image::Pixel second) {
    return (first.m_blue - second.m_blue) * (first.m_blue - second.m_blue) +
           (first.m_green - second.m_green) * (first.m_green - second.m_green) +
           (first.m_red - second.m_red) * (first.m_red - second.m_red);
}

double SeamCarver::GetPixelEnergy(size_t y, size_t x) const {
    const size_t width  = GetImageHeight();
    const size_t height = GetImageWidth();
    const size_t upX = (x + 1) % width, downX = (x - 1 + width) % width;
    const size_t upY = (y + 1) % height, downY = (y - 1 + height) % height;
    return sqrt(GetEnergy(m_image.GetPixel(upY, x), m_image.GetPixel(downY, x)) +
                GetEnergy(m_image.GetPixel(y, downX), m_image.GetPixel(y, upX)));
}

SeamCarver::Seam SeamCarver::FindVerticalSeam() const {
    Seam ans;
    int n = GetImageWidth();
    int m = GetImageHeight();
    std::vector<std::vector<double>> dp;
    for (int i = 0; i < n + 1; i++) {
        std::vector<double> v;
        v.resize(m + 1, 0);
        dp.push_back(v);
    }
    std::vector<std::vector<std::pair<int, int>>> p;
    for (int i = 0; i < n + 1; i++) {
        std::vector<std::pair<int, int>> v;
        v.resize(m + 1, {0, 0});
        p.push_back(v);
    }
    for (int j = 0; j <= m; j++) {
        for (int i = 0; i <= n; i++) {
            if (j == 0 && i < n) {
                dp[i][j] = 0;
            } else {
                dp[i][j] = 1e18;
            }
            p[i][j] = {-1, -1};
        }
    }
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            double x = GetPixelEnergy(i, j);
            if (dp[i][j + 1] > dp[i][j] + x) {
                dp[i][j + 1] = dp[i][j] + x;
                p[i][j + 1]  = {i, j};
            }
            if (dp[i + 1][j + 1] > dp[i][j] + x) {
                dp[i + 1][j + 1] = dp[i][j] + x;
                p[i + 1][j + 1]  = {i, j};
            }
            if (i > 0 && dp[i - 1][j + 1] > dp[i][j] + x) {
                dp[i - 1][j + 1] = dp[i][j] + x;
                p[i - 1][j + 1]  = {i, j};
            }
        }
    }
    double mn = 1e18;
    std::pair<int, int> x;
    for (int i = 0; i < n; i++) {
        if (mn > dp[i][m]) {
            mn = dp[i][m];
            x  = p[i][m];
        }
    }
    while (x.first != -1 && x.second != -1) {
        ans.push_back(x.first);
        x = p[x.first][x.second];
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

SeamCarver::Seam SeamCarver::FindHorizontalSeam() const {
    Seam ans;
    int n = GetImageWidth();
    int m = GetImageHeight();
    std::vector<std::vector<double>> dp;
    for (int i = 0; i < n + 1; i++) {
        std::vector<double> v;
        v.resize(m + 1, 0);
        dp.push_back(v);
    }
    std::vector<std::vector<std::pair<int, int>>> p;
    for (int i = 0; i < n + 1; i++) {
        std::vector<std::pair<int, int>> v;
        v.resize(m + 1, {0, 0});
        p.push_back(v);
    }
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            if (i == 0 && j < m) {
                dp[i][j] = 0;
            } else {
                dp[i][j] = 1e18;
            }
            p[i][j] = {-1, -1};
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            double x = GetPixelEnergy(i, j);
            if (dp[i + 1][j] > dp[i][j] + x) {
                dp[i + 1][j] = dp[i][j] + x;
                p[i + 1][j]  = {i, j};
            }
            if (dp[i + 1][j + 1] > dp[i][j] + x) {
                dp[i + 1][j + 1] = dp[i][j] + x;
                p[i + 1][j + 1]  = {i, j};
            }
            if (j > 0 && dp[i + 1][j - 1] > dp[i][j] + x) {
                dp[i + 1][j - 1] = dp[i][j] + x;
                p[i + 1][j - 1]  = {i, j};
            }
        }
    }
    double mn = 1e18;
    std::pair<int, int> x;
    for (int j = 0; j < m; j++) {
        if (mn > dp[n][j]) {
            mn = dp[n][j];
            x  = p[n][j];
        }
    }
    while (x.first != -1 && x.second != -1) {
        ans.push_back(x.second);
        x = p[x.first][x.second];
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

void SeamCarver::RemoveHorizontalSeam(const Seam &del) {
    int n = GetImageWidth(), m = GetImageHeight();
    const Image::Pixel negate = Image::Pixel(-1, -1, -1);
    for (int i = 0; i < (int)(del.size()); i++) {
        m_image.m_table[i][del[i]] = negate;
    }
    for (int i = 0; i < n; i++) {
        for (int j = m - 1; j > 0; j--) {
            if (m_image.m_table[i][j - 1].m_blue == -1) {
                m_image.m_table[i][j - 1] = m_image.m_table[i][j];
                m_image.m_table[i][j]     = negate;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        m_image.m_table[i].pop_back();
    }
    m_image = std::move(m_image.m_table);
}

void SeamCarver::RemoveVerticalSeam(const Seam &del) {
    int n = GetImageWidth(), m = GetImageHeight();
    const Image::Pixel negate                = Image::Pixel(-1, -1, -1);
    std::vector<std::vector<Image::Pixel>> v = m_image.m_table, nw;
    for (int i = 0; i < (int)(del.size()); i++) {
        m_image.m_table[del[i]][i] = negate;
    }
    for (int j = 0; j < m; j++) {
        for (int i = n - 1; i > 0; i--) {
            if (m_image.m_table[i - 1][j].m_blue == -1) {
                m_image.m_table[i - 1][j] = m_image.m_table[i][j];
                m_image.m_table[i][j]     = negate;
            }
        }
    }

    m_image.m_table.pop_back();
    m_image = std::move(m_image.m_table);
}
