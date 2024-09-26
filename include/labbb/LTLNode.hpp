#ifndef LABBB_LTLNODE_HPP
#define LABBB_LTLNODE_HPP

#include <cstddef>
#include <functional>
#include <map>
#include "types.hpp"
#include "unique_identifier.hpp"

namespace rvstd {

struct LTLNode {
  using node_type = unique_identifier; // what kind of node it is
  using identifier_type = unique_identifier; // unique identifier we give
  using relation_type = std::vector<identifier_type>; //
  using relations_type = std::unordered_map<unique_identifier, relation_type>;
  using attributes_type = json_value; // start & end time
  using type = LTLNode;

  LTLNode(
  node_type type, relations_type rels, attributes_type data = nullptr);

  // Labbb_NODISCARD
  auto type() const noexcept -> node_type;

  // Labbb_NODISCARD
  auto relations() const noexcept -> relations_type const&;

  // Labbb_NODISCARD
  auto attributes() const noexcept -> attributes_type const&;


  // operations
  friend bool operator==(LTLNode const& lhs, LTLNode const& rhs) noexcept;
  friend bool operator!=(LTLNode const& lhs, LTLNode const& rhs) noexcept;

 private:
  node_type type_;
  relations_type rels_;
  attributes_type data_;

};

} // namespace rvstd


namespace std {

template<>
struct hash<rvstd::LTLNode> {
  std::size_t operator()(rvstd::LTLNode const& obj) const noexcept;
};

template<>
struct equal_to<rvstd::LTLNode> {
  bool operator()(
    ::rvstd::LTLNode const& lhs, ::rvstd::LTLNode const& rhs) const noexcept;
};

}  // namespace std
#endif
