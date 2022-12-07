#ifndef AOC_FAKEFILESYSTEM_H
#define AOC_FAKEFILESYSTEM_H

#include <optional>
#include <vector>
#include <memory>
#include <iostream>

namespace aoc {

class FakeFS;

class FakeFS{
public:
	using ind_t = unsigned;
	using name_t = std::string;
	using dir_t = std::shared_ptr<FakeFS>;
	using file_t = std::pair<name_t, ind_t>;
	using child_t = std::pair<name_t, dir_t>;
	using file_iterator = std::vector<file_t>::const_iterator;
	using child_iterator = std::vector<child_t>::const_iterator;

private:
	std::optional<std::weak_ptr<FakeFS>> parent_ = std::nullopt;
	std::vector<child_t> children_;
	std::vector<file_t> files_;
	std::optional<ind_t> contained_ = std::nullopt;

public:
	FakeFS() = default;
	explicit FakeFS(std::shared_ptr<FakeFS> p){
	  std::weak_ptr<FakeFS> parent = p;
		parent_ = parent;
	};

	void emplace_child(const name_t &, dir_t);
	void emplace_file(const name_t &, ind_t);
	bool add_child(child_t);
	bool add_file(file_t);

	ind_t command_du() const;
	ind_t walk_du();
	dir_t command_cd(const std::string & name);
	void command_ls();

	ind_t size() {
		if (!contained_.has_value()) contained_ = walk_du();
		return contained_.value();
	}

	std::optional<ind_t> contained() {return contained_;}
	bool has_contained() const {return contained_.has_value();}
	ind_t contained_value() const {return contained_.value();}
	bool is_root() const {return (parent_.has_value()) ? false : true;}


	file_iterator f_begin() const;
	file_iterator f_end() const;
	
	child_iterator c_begin() const;
	child_iterator c_end() const;

	ind_t count_children_larger_than(const ind_t) const;
	ind_t count_children_smaller_than(const ind_t) const;
	ind_t sum_children_smaller_than(const ind_t) const;
	ind_t optimal_child_size(const ind_t, const ind_t) const;

friend std::ostream & operator<<(std::ostream & os, const aoc::FakeFS & fs){
	if (fs.has_contained()){
		os << "[" << fs.contained_value() << "]";
	}
  if (fs.is_root()) os << "/";
	os << "\n";
  for (auto c = fs.c_begin(); c != fs.c_end(); ++c) os << (c->first) << *(c->second);
  for (auto f = fs.f_begin(); f != fs.f_end(); ++f) os << (f->first) << "(size=" << f->second << ")\n";
  return os;
}

};

void fakefilesystem_add_content(std::shared_ptr<FakeFS> node, std::vector<std::string>& lines);
void fakefilesystem_inserter(std::shared_ptr<aoc::FakeFS> ffs, const std::vector<std::string>& lines);

} // namespace aoc


#endif

