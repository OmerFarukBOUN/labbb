#include "LTLNode.hpp"
#include <boost/container_hash/hash.hpp>

namespace rvstd {

LTLNode::LTLNode(
  node_type type, relations_type rels, attributes_type data)
    : type_(type), rels_(std::move(rels)), data_(std::move(data))
{
}

auto LTLNode::type() const -> identifier_type
{
  return type_;
}

auto LTLNode::relations() const -> relations_type const&
{
  return rels_;
}

auto LTLNode::attributes() const -> attributes_type const&
{
  return data_;
}

auto hash_value(LTLNode const& src) -> std::size_t // It should work
{
  std::size_t seed = 0;

  boost::hash_combine(seed, src.type());
  boost::hash_combine(seed, src.relations());
  boost::hash_combine(seed, src.attributes());

  return seed;
}

// Comparison operators
bool operator==(LTLNode const& lhs, LTLNode const& rhs) noexcept // It should work
{
  return lhs.type_ == rhs.type_ && lhs.rels_ == rhs.rels_ &&
         lhs.data_ == rhs.data_;
}
bool operator!=(LTLNode const& lhs, LTLNode const& rhs) noexcept // It should work
{
  return !(lhs == rhs);
}

}  // namespace rvstd

auto std::hash<::rvstd::LTLNode>::operator()(
  ::rvstd::LTLNode const& obj) const noexcept -> size_t
{
  return ::boost::hash<::rvstd::LTLNode>{}(obj);
}

bool std::equal_to<::rvstd::LTLNode>::operator()(
  ::rvstd::LTLNode const& lhs, ::rvstd::LTLNode const& rhs) const noexcept
{
  return lhs == rhs;
}
