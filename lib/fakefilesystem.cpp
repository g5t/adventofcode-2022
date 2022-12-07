
#include "fakefilesystem.h"

void aoc::FakeFS::emplace_child(const name_t & name, dir_t dir){
  children_.emplace_back(name, dir);
}
void aoc::FakeFS::emplace_file(const name_t & name, ind_t size){
  files_.emplace_back(name, size);
}

bool aoc::FakeFS::add_child(child_t ch){
  children_.push_back(ch);
	return true;
}

bool aoc::FakeFS::add_file(file_t fn){
  files_.push_back(fn);
	return true;
}

//dir_t & aoc::FakeFS::get_child(const std::string & name){
//  for (auto & ch: children_) if (ch.first.compare(name) == 0) return ch.second;
//  
//  throw std::runtime_error("No known child");
//}
//
//file_t & aoc::FakeFS::get_file(const std::string & name){
//  for (auto & fn: files_) if (fn.first.compare(name) == 0) return fn;
//
//  throw std::runtime_error("No known file");
//}

aoc::FakeFS::ind_t aoc::FakeFS::command_du() const {
  aoc::FakeFS::ind_t total{0};

  for (auto & ch: children_) total += ch.second->command_du();
  for (auto & fn: files_) total += fn.second;

  return total;
}

aoc::FakeFS::ind_t aoc::FakeFS::walk_du() {
	// Shortcut evaluating the tree twice
	if (contained_.has_value()) return contained_.value();
  aoc::FakeFS::ind_t total{0};
	for (auto & fn: files_) total += fn.second;
	for (auto & ch: children_) total += ch.second->walk_du();
	contained_ = total;
	return total;
}


aoc::FakeFS::dir_t aoc::FakeFS::command_cd(const std::string & name) {
  if (name.compare("/") == 0){
	  if (parent_.has_value()) {
			aoc::FakeFS::dir_t dir = parent_.value().lock();
			if ((*dir).is_root()) return dir;
			return (*dir).command_cd(name);
		}
		throw std::runtime_error("cd / called directly on root");
  }
  if (name.compare("..") == 0){
	  if (parent_.has_value()) {
			auto dir = parent_.value().lock();
			return dir;
		}
		throw std::runtime_error("cd .. called directly on root");
  }

  for (auto & ch: children_) if (ch.first.compare(name) == 0) return ch.second;

	std::cout << *this;

	std::string msg = "Unknown directory='";
	msg += name + "'";
  throw std::runtime_error(msg);
}

void aoc::FakeFS::command_ls() {
  for (const auto & c: children_) std::cout << "dir " << c.first << "\n";
  for (const auto & f: files_) std::cout << f.second << " " << f.first << "\n";
}

aoc::FakeFS::child_iterator aoc::FakeFS::c_begin() const {return children_.begin();}
aoc::FakeFS::child_iterator aoc::FakeFS::c_end() const {return children_.end();}

aoc::FakeFS::file_iterator aoc::FakeFS::f_begin() const {return files_.begin();}
aoc::FakeFS::file_iterator aoc::FakeFS::f_end() const {return files_.end();}



aoc::FakeFS::ind_t aoc::FakeFS::count_children_larger_than(const aoc::FakeFS::ind_t size) const {
  ind_t count{0};
	// this requires that the tree has already been walked to fill in contained sizes
	for (const auto & c: children_) if (c.second->contained() && c.second->contained().value() > size) {
	  count += 1 + c.second->count_children_larger_than(size);
	}
	return count;
}

aoc::FakeFS::ind_t aoc::FakeFS::count_children_smaller_than(const aoc::FakeFS::ind_t size) const {
  ind_t count{0};
	// this requires that the tree has already been walked to fill in contained sizes
	for (const auto & c: children_){
	  auto cont = c.second->contained();
		if (cont && cont.value() < size) ++count;
		count += c.second->count_children_smaller_than(size);
	}
	return count;
}

aoc::FakeFS::ind_t aoc::FakeFS::sum_children_smaller_than(const aoc::FakeFS::ind_t size) const {
  ind_t total{0};
	// this requires that the tree has already been walked to fill in contained sizes
	for (const auto & c: children_){
	  auto cont = c.second->contained();
		if (cont && cont.value() < size) total += cont.value();
		total += c.second->sum_children_smaller_than(size);
	}
	return total;
}

aoc::FakeFS::ind_t aoc::FakeFS::optimal_child_size(const aoc::FakeFS::ind_t size, const aoc::FakeFS::ind_t best) const {
	ind_t opt{best};
	for (const auto & c: children_){
	  auto cc = c.second->contained();
		if (cc.has_value() && cc.value() >= size && cc.value() < opt) opt = cc.value();
		opt = c.second->optimal_child_size(size, opt);
	}
	return opt;
}


void aoc::fakefilesystem_add_content(std::shared_ptr<aoc::FakeFS> node, std::vector<std::string>& lines){
 for (const auto & line: lines){
   if (line.compare(0, 3, "dir") == 0){
	   auto name = std::string(line.substr(4));
	   auto dir = std::make_shared<aoc::FakeFS>(node);
		 node->emplace_child(name, dir);
   } else { 
     auto pos = line.find(" ");
     auto size = static_cast<aoc::FakeFS::ind_t>(std::stoi(line.substr(0, pos)));
     auto name = std::string(line.substr(pos+1));
		 node->emplace_file(name, size);
   }
 }
}

void aoc::fakefilesystem_inserter(std::shared_ptr<aoc::FakeFS> ffs, const std::vector<std::string>& lines){
	  std::vector<std::string> store; 
	  for (const auto & line: lines){
		 if (line[0] == '$') {
    	  if (line.compare(0,4, "$ cd") == 0) {
				  if (!store.empty())
						aoc::fakefilesystem_add_content(ffs, store);
					auto new_ffs = ffs->command_cd(line.substr(5));
					new_ffs.swap(ffs);

					// the store is only added to *after* and `ls` 
					// and is pushed to the current directory just before a `cd`
					store.clear();
				}
				// if `ls` *should* be a non-op at this point
			} else {
			  store.push_back(line);
		  }
		}
		if (!store.empty()) aoc::fakefilesystem_add_content(ffs, store);
}
