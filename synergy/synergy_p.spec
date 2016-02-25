Summary: install version of synerys
Name: synery
Version: 1.0
Release: 1.1%{?dist}
URL: http://www.openssh.com/portable.html

Source0: readme.txt
Source1: synergy-1.3.6-1.el6.rf.x86_64.rpm
Source2: synergy.conf
Source3: synergys

License: BSD
Group: Applications/Internet
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

Requires(post): chkconfig >= 0.9, /sbin/service, pidof
Requires(pre): rpm

%description
tools share mouse and keyboard between screens

%prep

%build

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p -m755 $RPM_BUILD_ROOT/synery

install -m644 %{SOURCE0} $RPM_BUILD_ROOT/etc/readme.txt
install -m644 %{SOURCE1} $RPM_BUILD_ROOT/%{SOURCE1}
install -m644 %{SOURCE2} $RPM_BUILD_ROOT/etc/synergy.conf
install -m644 %{SOURCE3} $RPM_BUILD_ROOT/etc/rc.d/init.d/synergys

%clean
rm -rf $RPM_BUILD_ROOT

%post 
rpm -i $RPM_BUILD_ROOT/%{SOURCE1}
#rm $RPM_BUILD_ROOT/%{SOURCE1}
/sbin/chkconfig --add synergys

%preun 
if [ "$1" = 0 ]
then
	/sbin/service synerys stop > /dev/null 2>&1 || :
	/sbin/chkconfig --del sshd
fi

%files
%defattr(-,root,root)
%attr(0600,root,root) %config(noreplace) %{_sysconfdir}/ssh/sshd_config
%attr(0644,root,root) %config(noreplace) /etc/synery.conf
%attr(0640,root,root) %config(noreplace) /etc/sysconfig/sshd
%attr(0755,root,root) /etc/rc.d/init.d/sshd

%changelog
* Thu Dec 13 2012 Petr Lautrbach <plautrba@redhat.com> 5.3p1-84.1
- Add a 'netcat mode' (ssh -W) (#860809)
- improve ldap backend (#455350)
