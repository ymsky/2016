import os
import rospy
import rospkg


from qt_gui.plugin import Plugin
from python_qt_binding import loadUi
from python_qt_binding.QtGui import QWidget
from std_msgs.msg import UInt16


class MyPlugin(Plugin):


    def __init__(self, context):
        super(MyPlugin, self).__init__(context)
        # Give QObjects reasonable names
        self.setObjectName('MyPluginPy')
	
        self._publisher = rospy.Publisher('command_request',UInt16,queue_size=10)


        # Process standalone plugin command-line arguments
        from argparse import ArgumentParser
        parser = ArgumentParser()
        # Add argument(s) to the parser.
        parser.add_argument("-q", "--quiet", action="store_true",
                      dest="quiet",
                      help="Put plugin in silent mode")
        args, unknowns = parser.parse_known_args(context.argv())
        if not args.quiet:
            print 'arguments: ', args
            print 'unknowns: ', unknowns

        # Create QWidget
        self._widget = QWidget()
        # Get path to UI file which is a sibling of this file
        # in this example the .ui and .py file are in the same folder
        ui_file = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'MyPlugin1.ui')
        # Extend the widget with all attributes and children from UI file
        loadUi(ui_file, self._widget)
        # Give QObjects reasonable names
        self._widget.setObjectName('MyPluginUi')
        # Show _widget.windowTitle on left-top of each plugin (when 
        # it's set in _widget). This is useful when you open multiple 
        # plugins at once. Also if you open multiple instances of your 
        # plugin at once, these lines add number to make it easy to 
        # tell from pane to pane.
        if context.serial_number() > 1:
            self._widget.setWindowTitle(self._widget.windowTitle() + (' (%d)' % context.serial_number()))
        # Add widget to the user interface
        context.add_widget(self._widget)

	self._widget.radioButton_1.clicked.connect(self._on_RB1_checked)
	self._widget.radioButton_2.clicked.connect(self._on_RB2_checked)
	self._widget.radioButton_3.clicked.connect(self._on_RB3_checked)
	self._widget.pushButton_1.pressed.connect(self._on_PB1_pressed)
	self._widget.pushButton_2.pressed.connect(self._on_PB2_pressed)
	self._widget.pushButton_3.pressed.connect(self._on_PB3_pressed)
	self._widget.pushButton_4.pressed.connect(self._on_PB4_pressed)
	self._widget.pushButton_5.pressed.connect(self._on_PB5_pressed)
	self._widget.pushButton_6.pressed.connect(self._on_PB6_pressed)
	self._widget.pushButton_7.pressed.connect(self._on_PB7_pressed)
	self._widget.pushButton_8.pressed.connect(self._on_PB8_pressed)
	self._widget.pushButton_9.pressed.connect(self._on_PB9_pressed)
	self._widget.pushButton_10.pressed.connect(self._on_PB10_pressed)
	self._widget.pushButton_11.pressed.connect(self._on_PB11_pressed)
	self._widget.pushButton_12.pressed.connect(self._on_PB12_pressed)

	self.m_index=1
	self.b_gui_flag=1

	
		
    def _on_PB1_pressed(self):
	self.m_index=1
	print 'stop'
	self._publisher.publish(self.m_index)	
	
    def _on_PB2_pressed(self):
	self.m_index=2
	print 'move forward'
	self._publisher.publish(self.m_index)	

    def _on_PB3_pressed(self):
	self.m_index=3
	print 'move backward'
	self._publisher.publish(self.m_index)	

    def _on_PB4_pressed(self):
	self.m_index=4
	print 'move left'
	self._publisher.publish(self.m_index)	

    def _on_PB5_pressed(self):
	self.m_index=5
	print 'move right'
	self._publisher.publish(self.m_index)	

    def _on_PB6_pressed(self):
	self.m_index=6
	print 'go up'
	self._publisher.publish(self.m_index)	

    def _on_PB7_pressed(self):
	self.m_index=7
	print 'go down'
	self._publisher.publish(self.m_index)	

    def _on_PB8_pressed(self):
	self.m_index=8
	print 'turn left'
	self._publisher.publish(self.m_index)	

    def _on_PB9_pressed(self):
	self.m_index=9
	print 'turn right'
	self._publisher.publish(self.m_index)	

    def _on_PB10_pressed(self):
	self.m_index=10
	print 'take off'
	self._publisher.publish(self.m_index)	

    def _on_PB11_pressed(self):
	self.m_index=11
	print 'land'
	self._publisher.publish(self.m_index)	

    def _on_PB12_pressed(self):
	self.m_index=12
	print 'reset'
	self._publisher.publish(self.m_index)	


    def _on_RB1_checked(self):
	self.m_index=100
	print 'uav1_only'
	self._publisher.publish(self.m_index)	

    def _on_RB2_checked(self):
	self.m_index=101
	print 'uav2_only'
	self._publisher.publish(self.m_index)	
	
    def _on_RB3_checked(self):
	self.m_index=102
	print 'uav1_and_uav2'
	self._publisher.publish(self.m_index)	




    def shutdown_plugin(self):
        # TODO unregister all publishers here
        pass

    def save_settings(self, plugin_settings, instance_settings):
        # TODO save intrinsic configuration, usually using:
        # instance_settings.set_value(k, v)
        pass

    def restore_settings(self, plugin_settings, instance_settings):
        # TODO restore intrinsic configuration, usually using:
        # v = instance_settings.value(k)
        pass

    #def trigger_configuration(self):
        # Comment in to signal that the plugin has a way to configure
        # This will enable a setting button (gear icon) in each dock widget title bar
        # Usually used to open a modal configuration dialog
