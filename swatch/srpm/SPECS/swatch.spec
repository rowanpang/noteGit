Name:           swatch
Version:        3.2.3
Release:        7%{?dist}
Summary:        Tool for actively monitoring log files
Group:          Applications/File
License:        GPLv2+
URL:            http://swatch.sourceforge.net/
Source0:        http://download.sf.net/swatch/swatch-%{version}.tar.gz
BuildArch:      noarch
BuildRequires:  perl(Date::Calc)
BuildRequires:  perl(Date::Format)
BuildRequires:  perl(Date::Manip)
BuildRequires:  perl(File::Tail)
BuildRequires:  perl(ExtUtils::MakeMaker)
Requires:       perl(:MODULE_COMPAT_%(eval "`%{__perl} -V:version`"; echo $version))
Patch0:         swatch-3.2.3-manpage-fix.patch

%description
The Simple WATCHer is an automated monitoring tool that is capable
of alerting system administrators of anything that matches the
patterns described in the configuration file, whilst constantly
searching logfiles using perl.

%prep
%setup -q
%patch0 -p1 -b .fix
chmod -v 644 tools/*

%{?filter_from_requires: %filter_from_requires /^perl(Mail:Sendmail)$/d }
%{?filter_from_requires: %filter_from_requires /^perl(Sys:Hostname)$/d }
%{?perl_default_filter}

%build
perl Makefile.PL INSTALLDIRS=vendor
make %{?_smp_mflags}

%install
make pure_install PERL_INSTALL_ROOT=%{buildroot}
find %{buildroot} -type f -name .packlist  -exec rm -f {} ';'
find %{buildroot} -type d -depth -exec rmdir {} 2>/dev/null ';'
chmod -R u+w $RPM_BUILD_ROOT/*

%check
make test

%files
%defattr(-,root,root,-)
%doc CHANGES COPYRIGHT COPYING KNOWN_BUGS README examples/ tools/
%{_bindir}/swatch
%{_mandir}/man1/*.1*
%{_mandir}/man3/*.3pm*
%{perl_vendorlib}/Swatch/
%{perl_vendorlib}/auto/Swatch/

%changelog
* Fri Feb  8 2013 Tom Callaway <spot@fedoraproject.org> - 3.2.3-7
- fix manpage (bz 909120)

* Sat Jul 21 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 3.2.3-6
- Rebuilt for https://fedoraproject.org/wiki/Fedora_18_Mass_Rebuild

* Wed Jun 20 2012 Petr Pisar <ppisar@redhat.com> - 3.2.3-5
- Perl 5.16 rebuild

* Sat Jan 14 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 3.2.3-4
- Rebuilt for https://fedoraproject.org/wiki/Fedora_17_Mass_Rebuild

* Tue Jul 19 2011 Petr Sabata <contyk@redhat.com> - 3.2.3-3
- Perl mass rebuild

* Wed Feb 09 2011 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 3.2.3-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_15_Mass_Rebuild

* Tue Jul 29 2010 Tom "spot" Callaway <tcallawa@redhat.com> - 3.2.3-1
- update to 3.2.3

* Wed Jun 02 2010 Marcela Maslanova <mmaslano@redhat.com> - 3.2.1-6
- Mass rebuild with perl-5.12.0

* Mon Dec  7 2009 Stepan Kasal <skasal@redhat.com> - 3.2.1-5
- rebuild against perl 5.10.1

* Sun Jul 26 2009 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 3.2.1-4
- Rebuilt for https://fedoraproject.org/wiki/Fedora_12_Mass_Rebuild

* Wed Feb 25 2009 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 3.2.1-3
- Rebuilt for https://fedoraproject.org/wiki/Fedora_11_Mass_Rebuild

* Thu Mar 06 2008 Tom "spot" Callaway <tcallawa@redhat.com> - 3.2.1-2.1
Rebuild for new perl

* Wed Oct 17 2007 Tom "spot" Callaway <tcallawa@redhat.com> - 3.2.1-1.1
- correct license tag
- add BR: perl(ExtUtils::MakeMaker)

* Tue Sep  5 2006 Jose Pedro Oliveira <jpo at di.uminho.pt> - 3.2.1-1
- Update to 3.2.1.
- Dropped patch: swatch-3.1.1.patch.
- Filtered doc requirements.

* Thu Mar  2 2006 Jose Pedro Oliveira <jpo at di.uminho.pt> - 3.1.1-5
- Rebuild for FC5 (perl 5.8.8).

* Thu Jan  5 2006 Jose Pedro Oliveira <jpo at di.uminho.pt> - 3.1.1-4
- Patch supplied by John Horne: doc updates and $0 handling (#160817).

* Thu May 12 2005 Jose Pedro Oliveira <jpo at di.uminho.pt> - 3.1.1-3
- Add dist tag.

* Fri Apr  7 2005 Michael Schwendt <mschwendt[AT]users.sf.net> - 3.1.1-2
- rebuilt

* Thu Aug 26 2004 Gavin Henry <ghenry[AT]suretecsystems.com> - 0:3.1.1-1
- Retaking ownership, thanks jpo and updating for version 3.1.1

* Tue May 25 2004 Jose Pedro Oliveira <jpo at di.uminho.pt> - 0:3.1-0.fdr.3
- Taking temporary ownership.
- Synced specfile suggestions.
- Cleared the requirements.

* Mon May 10 2004 Gavin Henry <ghenry@suretecsystems.com> - 0:3.1-0.fdr.2
- Updated with bugzilla changes.

* Wed Apr 28 2004 Gavin Henry <ghenry@suretecsystems.com> - 0:3.1-0.fdr.1
- Updated for version 3.1

* Thu Mar 18 2004 Gavin Henry <ghenry@suretecsystems.com> - 0:3.0.8-0.fdr.1
- Updated with QA comments and added BuildRequires. Fedora.us - Bug 1346

* Sat Mar 06 2004 Gavin Henry <ghenry@suretecsystems.com> - 0:3.0.8-0
- Initial package release.
