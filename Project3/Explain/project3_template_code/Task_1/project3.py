from mininet.topo import Topo

class Project3_Topo( Topo ):
	def __init__( self ):
		Topo.__init__( self )
		#新增 host 節點
		h1 = self.addHost( 'h1', ip = '10.0.0.1', mac = '00:00:00:00:00:01' )
		h2 = self.addHost( 'h2', ip = '', mac = '' )
		h3 = self.addHost( 'h3', ip = '', mac = '' )
		h4 = self.addHost( 'h4', ip = '', mac = '' )

                #新增 switch 節點
		s1 = self.addSwitch( 's1' )
		s2 = self.addSwitch( '' )
		s3 = self.addSwitch( '' )
		s4 = self.addSwitch( '' )
		s5 = self.addSwitch( '' )
		s6 = self.addSwitch( '' )

		#新增節點 link
		self.addLink( h1, s1 )
		self.addLink( h2, s1 )
		self.addLink( s1, s2 )
		self.addLink( s2, s3 )
		self.addLink( s2, s4 )
		self.addLink( s3, s5 )
		self.addLink( s4, s6 )
		self.addLink( h3, s5 )
		self.addLink( h4, s6 )



topos = { 'project3': Project3_Topo }
