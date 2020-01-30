//
// Created by Татьяна on 2020-01-29.
//
#pragma once

void writeHashSum(boost::filesystem::path&, std::vector<std::vector<unsigned short> >&);

void singleThreadHasherTask(const boost::filesystem::path&, size_t&, size_t&, unsigned long&, std::vector<unsigned short>&);

void CheckPaths(const boost::filesystem::path&, const boost::filesystem::path&);