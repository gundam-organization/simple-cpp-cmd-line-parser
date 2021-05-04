//
// Created by Adrien BLANCHET on 03/05/2021.
//

#ifndef SIMPLE_CPP_OPTION_PARSER_OPTIONHOLDER_H
#define SIMPLE_CPP_OPTION_PARSER_OPTIONHOLDER_H

#include "string"
#include "sstream"
#include <iomanip>
#include <iostream>

class OptionHolder {

public:
  OptionHolder() = default;
  virtual ~OptionHolder() = default;

  void setName(const std::string &name) {
    _name_ = name;
  }
  void setDescription(const std::string &description) {
    _description_ = description;
  }
  void addCmdLineCallStr(const std::string &cmdLineCallStr){
    _cmdLineCallStrList_.emplace_back(cmdLineCallStr);
  }
  void setNbExpectedVars(int nbExpectedVars_){
    _nbExpectedVars_ = nbExpectedVars_;
  }
  void setIsTriggered(bool isTriggered_){
    _isTriggered_ = isTriggered_;
  }
  void setNextVariableValue(const std::string& valueStr_){
    if( _nbExpectedVars_ == -1 or _strValuesList_.size() < _nbExpectedVars_ ){
      _strValuesList_.emplace_back(valueStr_);
    }
    else{
      throw std::logic_error( _name_ + ": Can't set more values than the specified expected nb of var." );
    }
  }

  const std::string &getName() const {
    return _name_;
  }
  const std::string &getDescription() const {
    return _description_;
  }
  const std::vector<std::string> &getCmdLineCallStrList() const {
    return _cmdLineCallStrList_;
  }
  bool isFullyFilled() const {
    if( _nbExpectedVars_ != -1 ) return ( _nbExpectedVars_ == _strValuesList_.size() );
    else return true;
  }
  const std::vector<std::string> &getStrValuesList() const {
    return _strValuesList_;
  }
  size_t getVarListSize() const { return _strValuesList_.size(); }
  int getNbExpectedVars() const { return _nbExpectedVars_; }
  bool isTriggered() const { return _isTriggered_; }
  template<typename T> T getValue(size_t index_) const {
    if( index_ >= _strValuesList_.size() ){
      if( _nbExpectedVars_ >= 2 ){
        throw std::logic_error(_name_ + ": Value #" + std::to_string(index_) + " not set." );
      }
      else if( _nbExpectedVars_ == 1 ){
        throw std::logic_error(_name_ + ": Value not set." );
      }
      else if( _nbExpectedVars_ == 0){
        throw std::logic_error(_name_ + ": is a trigger option, not value can be fetch. Use isTriggered() instead." );
      }
      else{
        throw std::logic_error(_name_ + ": Value #" + std::to_string(index_) + " can't be fectch since var size is " + std::to_string(_strValuesList_.size()) );
      }
    }
    T outputVar;
    std::stringstream  sStream(_strValuesList_.at(index_));
    sStream >> outputVar;

    // Sanity check
    std::stringstream sStreamLoopBack;
    sStreamLoopBack.precision(int(_strValuesList_.at(index_).size()));
    sStreamLoopBack << outputVar;

    if( sStreamLoopBack.str() != sStream.str() ){
      throw std::runtime_error( _name_ + ": Could not convert \"" + sStream.str() + "\" to specified type. (Could be double precision involved)");
    }

    return outputVar;
  }
  template<> double getValue(size_t index_) const {
    // Override to bypass
    if( index_ >= _strValuesList_.size() ){
      throw std::logic_error(_name_ + ": Could not fetch index value: " + std::to_string(index_) + " when var list size is " + std::to_string(_strValuesList_.size()) );
    }
    return std::stod(_strValuesList_.at(index_));
  }
  template<> float getValue(size_t index_) const {
    if( index_ >= _strValuesList_.size() ){
      throw std::logic_error(_name_ + ": Could not fetch index value: " + std::to_string(index_) + " when var list size is " + std::to_string(_strValuesList_.size()) );
    }
    return std::stof(_strValuesList_.at(index_));
  }
  template<> std::string getValue(size_t index_) const {
    if( index_ >= _strValuesList_.size() ){
      throw std::logic_error(_name_ + ": Could not fetch index value: " + std::to_string(index_) + " when var list size is " + std::to_string(_strValuesList_.size()) );
    }
    return _strValuesList_.at(index_);
  }

  std::string getSummary() const{
    std::stringstream ss;
    ss << _name_ << " {";
    for(auto it = _cmdLineCallStrList_.begin(); it != _cmdLineCallStrList_.end(); ++it) {
      ss << *it;
      if(std::next(it) != _cmdLineCallStrList_.end()) {
        ss << ",";
      }
    }
    ss << "}: " << _description_;
    if( _nbExpectedVars_ > 0 ){
      ss << " (" << _nbExpectedVars_ << " value(s) expected)";
    }
    else if( _nbExpectedVars_ == 0 ){
      ss << " (trigger)";
    }
    else{
      ss << " (as many values as you provide)";
    }

    return ss.str();
  }

private:

  bool _isTriggered_{false};
  std::string _name_;
  std::string _description_;
  std::vector<std::string> _cmdLineCallStrList_;
  int _nbExpectedVars_{-1}; // default is infinite
  std::vector<std::string> _strValuesList_; // Don't need to store multiple variable types since every argument comes from a char*...


};

// Deprecated tho (not necessary, but kept here in case I need it :) ):
// Inspired from https://stackoverflow.com/questions/18566066/c-elegant-way-of-holding-different-types-of-data-in-a-member-variable

#endif //SIMPLE_CPP_OPTION_PARSER_OPTIONHOLDER_H
