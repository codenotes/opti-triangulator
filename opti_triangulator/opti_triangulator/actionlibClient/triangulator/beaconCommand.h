// Generated by gencpp from file triangulator/beaconCommand.msg
// DO NOT EDIT!


#ifndef TRIANGULATOR_MESSAGE_BEACONCOMMAND_H
#define TRIANGULATOR_MESSAGE_BEACONCOMMAND_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace triangulator
{
template <class ContainerAllocator>
struct beaconCommand_
{
  typedef beaconCommand_<ContainerAllocator> Type;

  beaconCommand_()
    : commandID(0)
    , returnCode(0)
    , commandMessage()
    , floatArgs()
    , intArgs()
    , stringArgs()  {
    }
  beaconCommand_(const ContainerAllocator& _alloc)
    : commandID(0)
    , returnCode(0)
    , commandMessage(_alloc)
    , floatArgs(_alloc)
    , intArgs(_alloc)
    , stringArgs(_alloc)  {
    }



   typedef uint32_t _commandID_type;
  _commandID_type commandID;

   typedef uint32_t _returnCode_type;
  _returnCode_type returnCode;

   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _commandMessage_type;
  _commandMessage_type commandMessage;

   typedef std::vector<float, typename ContainerAllocator::template rebind<float>::other >  _floatArgs_type;
  _floatArgs_type floatArgs;

   typedef std::vector<int32_t, typename ContainerAllocator::template rebind<int32_t>::other >  _intArgs_type;
  _intArgs_type intArgs;

   typedef std::vector<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > , typename ContainerAllocator::template rebind<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::other >  _stringArgs_type;
  _stringArgs_type stringArgs;




  typedef boost::shared_ptr< ::triangulator::beaconCommand_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::triangulator::beaconCommand_<ContainerAllocator> const> ConstPtr;

}; // struct beaconCommand_

typedef ::triangulator::beaconCommand_<std::allocator<void> > beaconCommand;

typedef boost::shared_ptr< ::triangulator::beaconCommand > beaconCommandPtr;
typedef boost::shared_ptr< ::triangulator::beaconCommand const> beaconCommandConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::triangulator::beaconCommand_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::triangulator::beaconCommand_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace triangulator

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'triangulator': ['/home/gbrill/catkin_ws/src/triangulator/msg'], 'std_msgs': ['/opt/ros/indigo/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::triangulator::beaconCommand_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::triangulator::beaconCommand_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::triangulator::beaconCommand_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::triangulator::beaconCommand_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::triangulator::beaconCommand_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::triangulator::beaconCommand_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::triangulator::beaconCommand_<ContainerAllocator> >
{
  static const char* value()
  {
    return "dc4662da9c521d4b085b6a1b36d58e74";
  }

  static const char* value(const ::triangulator::beaconCommand_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xdc4662da9c521d4bULL;
  static const uint64_t static_value2 = 0x085b6a1b36d58e74ULL;
};

template<class ContainerAllocator>
struct DataType< ::triangulator::beaconCommand_<ContainerAllocator> >
{
  static const char* value()
  {
    return "triangulator/beaconCommand";
  }

  static const char* value(const ::triangulator::beaconCommand_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::triangulator::beaconCommand_<ContainerAllocator> >
{
  static const char* value()
  {
    return "uint32 commandID\n\
uint32 returnCode\n\
string commandMessage\n\
float32[] floatArgs\n\
int32[] intArgs\n\
string[] stringArgs\n\
";
  }

  static const char* value(const ::triangulator::beaconCommand_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::triangulator::beaconCommand_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.commandID);
      stream.next(m.returnCode);
      stream.next(m.commandMessage);
      stream.next(m.floatArgs);
      stream.next(m.intArgs);
      stream.next(m.stringArgs);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER;
  }; // struct beaconCommand_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::triangulator::beaconCommand_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::triangulator::beaconCommand_<ContainerAllocator>& v)
  {
    s << indent << "commandID: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.commandID);
    s << indent << "returnCode: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.returnCode);
    s << indent << "commandMessage: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.commandMessage);
    s << indent << "floatArgs[]" << std::endl;
    for (size_t i = 0; i < v.floatArgs.size(); ++i)
    {
      s << indent << "  floatArgs[" << i << "]: ";
      Printer<float>::stream(s, indent + "  ", v.floatArgs[i]);
    }
    s << indent << "intArgs[]" << std::endl;
    for (size_t i = 0; i < v.intArgs.size(); ++i)
    {
      s << indent << "  intArgs[" << i << "]: ";
      Printer<int32_t>::stream(s, indent + "  ", v.intArgs[i]);
    }
    s << indent << "stringArgs[]" << std::endl;
    for (size_t i = 0; i < v.stringArgs.size(); ++i)
    {
      s << indent << "  stringArgs[" << i << "]: ";
      Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.stringArgs[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // TRIANGULATOR_MESSAGE_BEACONCOMMAND_H
