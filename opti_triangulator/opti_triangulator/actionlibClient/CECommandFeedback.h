// Generated by gencpp from file tests/CECommandFeedback.msg
// DO NOT EDIT!


#ifndef TESTS_MESSAGE_CECOMMANDFEEDBACK_H
#define TESTS_MESSAGE_CECOMMANDFEEDBACK_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace tests
{
template <class ContainerAllocator>
struct CECommandFeedback_
{
  typedef CECommandFeedback_<ContainerAllocator> Type;

  CECommandFeedback_()
    : percent_complete(0.0)  {
    }
  CECommandFeedback_(const ContainerAllocator& _alloc)
    : percent_complete(0.0)  {
    }



   typedef float _percent_complete_type;
  _percent_complete_type percent_complete;




  typedef boost::shared_ptr< ::tests::CECommandFeedback_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::tests::CECommandFeedback_<ContainerAllocator> const> ConstPtr;

}; // struct CECommandFeedback_

typedef ::tests::CECommandFeedback_<std::allocator<void> > CECommandFeedback;

typedef boost::shared_ptr< ::tests::CECommandFeedback > CECommandFeedbackPtr;
typedef boost::shared_ptr< ::tests::CECommandFeedback const> CECommandFeedbackConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::tests::CECommandFeedback_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::tests::CECommandFeedback_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace tests

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'tests': ['/home/gbrill/ws2/src/tests/msg', '/home/gbrill/ws2/devel/share/tests/msg'], 'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg'], 'actionlib_msgs': ['/opt/ros/indigo/share/actionlib_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::tests::CECommandFeedback_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::tests::CECommandFeedback_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::tests::CECommandFeedback_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::tests::CECommandFeedback_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::tests::CECommandFeedback_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::tests::CECommandFeedback_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::tests::CECommandFeedback_<ContainerAllocator> >
{
  static const char* value()
  {
    return "d342375c60a5a58d3bc32664070a1368";
  }

  static const char* value(const ::tests::CECommandFeedback_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xd342375c60a5a58dULL;
  static const uint64_t static_value2 = 0x3bc32664070a1368ULL;
};

template<class ContainerAllocator>
struct DataType< ::tests::CECommandFeedback_<ContainerAllocator> >
{
  static const char* value()
  {
    return "tests/CECommandFeedback";
  }

  static const char* value(const ::tests::CECommandFeedback_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::tests::CECommandFeedback_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n\
# Define a feedback message\n\
float32 percent_complete\n\
\n\
";
  }

  static const char* value(const ::tests::CECommandFeedback_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::tests::CECommandFeedback_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.percent_complete);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct CECommandFeedback_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::tests::CECommandFeedback_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::tests::CECommandFeedback_<ContainerAllocator>& v)
  {
    s << indent << "percent_complete: ";
    Printer<float>::stream(s, indent + "  ", v.percent_complete);
  }
};

} // namespace message_operations
} // namespace ros

#endif // TESTS_MESSAGE_CECOMMANDFEEDBACK_H
