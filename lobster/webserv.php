<?php
define('PATH', '../');
require(PATH . "utils/check_session.php");
require(PATH . "utils/show_popup.php");
require(PATH . "utils/menu.php");
?>

<!DOCTYPE html>
<html lang="en">

<head>
	<meta content="text/html; charset=utf-8" http-equiv="Content-Type" />
	<meta
		content="width=device-width,height=device-height,user-scalable=no,initial-scale=1.0,maximum-scale=1.0,minimum-scale=1.0"
		name="viewport">
	<title>webserv correction</title>
	<link rel="shortcut icon" href="<?php echo PATH; ?>assets/favicons/favicon_no-bg.ico" type="image/x-icon">
	<link href="<?php echo PATH; ?>css/styles.css" media="all" rel="stylesheet" />
	<link rel="manifest" href="<?php echo PATH; ?>manifest.json">
	<script src="<?php echo PATH; ?>js/script.js"></script>
	<script src="<?php echo PATH; ?>js/js"></script>
	<script src="<?php echo PATH; ?>js/main.js"></script>
</head>
Check the code and ask questions

    Launch the installation of siege with homebrew.
    Ask explanations about the basics of an HTTP server.
    Ask what function the group used for I/O Multiplexing.
    Ask for an explanation of how does select() (or equivalent) work.
    Ask if they use only one select() (or equivalent) and how they've managed the server to accept and the client to read/write.
    The select() (or equivalent) should be in the main loop and should check file descriptors for read and write AT THE SAME TIME. If not, the grade is 0 and the evaluation process ends now.
    There should be only one read or one write per client per select() (or equivalent). Ask the group to show you the code from the select() (or equivalent) to the read and write of a client.
    Search for all read/recv/write/send on a socket and check that, if an error is returned, the client is removed.
    Search for all read/recv/write/send and check if the returned value is correctly checked (checking only -1 or 0 values is not enough, both should be checked).
    If errno is checked after read/recv/write/send, the grade is 0 and the evaluation process ends now.
    Writing or reading ANY file descriptor without going through the select() (or equivalent) is strictly FORBIDDEN.
    The project must compile without any re-link issue. If not, use the 'Invalid compilation' flag.
    If any point is unclear or is not correct, the evaluation stops.


	<script>
		if ('serviceWorker' in navigator) {
			navigator.serviceWorker.register('<?php echo PATH; ?>js/service-worker.js')
				.then(function (registration) {
					console.log('Service Worker registered successfully:', registration);
				})
				.catch(function (error) {
					console.log('Service Worker registration failed:', error);
				});
		}
	</script>
	<?php require(PATH . "utils/header.php"); ?>
	<div class="container-item scale-team-container">
		<div class="row mt-4">
			<div class="scale-section-infos col-sm-12 scale-div">
				<h3 class="font-weight-bold text-uppercase text-center mt-4 pt-4">
					Scale for project

					<a target="_blank" href="https://projects.intra.42.fr/projects/webserv">webserv</a>
				</h3>
				<h5 class="text-muted text-center">You should evaluate 3 students in this team</h5>

				<hr>
				<div class="p-0 scale-introduction dancer from-down" id="introduction-25879">
					<h3 class="font-weight-bold mb-1">Introduction</h3>
					<p>
						Please comply with the following rules:<br><br>- Remain polite, courteous, respectful and
						constructive
						throughout the <br> evaluation process. The well-being of the community depends on it.<br><br>-
						Identify
						with the student or group whose work is evaluated the possible <br> dysfunctions in their
						project. Take the
						time to discuss and debate the <br> problems that may have been identified.<br><br>- You must
						consider that
						there might be some differences in how your peers <br> might have understood the project's
						instructions and
						the scope of its <br> functionalities. Always keep an open mind and grade them as honestly as
						<br> possible.
						The pedagogy is useful only and only if the peer-evaluation is <br> done seriously.<br>
					</p>
				</div>
				<div class="p-0 scale-guidelines dancer from-down" id="guidelines-25879">
					<h3 class="font-weight-bold mb-1">Guidelines</h3>
					<p>
						- Only grade the work that was turned in the Git repository of the evaluated <br> student or
						group.<br><br>-
						Double-check that the Git repository belongs to the student(s). Ensure that <br> the project is
						the one
						expected. Also, check that 'git clone' is used in an <br> empty folder.<br><br>- Check carefully
						that no
						malicious aliases was used to fool you and make you <br> evaluate something that is not the
						content of the
						official repository.<br><br>- To avoid any surprises and if applicable, review together any
						scripts used
						<br> to facilitate the grading (scripts for testing or automation).<br><br>- If you have not
						completed the
						assignment you are going to evaluate, you have <br> to read the entire subject prior to starting
						the
						evaluation process.<br><br>- Use the available flags to report an empty repository, a
						non-functioning <br>
						program, a Norm error, cheating, and so forth. <br> In these cases, the evaluation process ends
						and the
						final grade is 0, <br> or -42 in case of cheating. However, except for cheating, student are
						<br> strongly
						encouraged to review together the work that was turned in, in order <br> to identify any
						mistakes that
						shouldn't be repeated in the future.<br><br>- Remember that for the duration of the defence, no
						segfault, no
						other <br> unexpected, premature, uncontrolled or unexpected termination of the <br> program,
						else the final
						grade is 0. Use the appropriate flag. <br> You should never have to edit any file except the
						configuration
						file if it <br> exists. If you want to edit a file, take the time to explicit the reasons <br>
						with the
						evaluated student and make sure both of you are okay with this.<br><br>- You must also verify
						the absence of
						memory leaks. Any memory allocated on <br> the heap must be properly freed before the end of
						execution. <br>
						You are allowed to use any of the different tools available on the computer, <br> such as leaks,
						valgrind,
						or e_fence. In case of memory leaks, tick the <br> appropriate flag.<br>
					</p>
				</div>

				<hr>
				<div class="dancer from-down" id="attachments-25879">
					<h3 class="font-weight-bold mb-1">Attachments</h3>
					<div class="project-attachments-list">
					</div>

					<div class="project-attachments-list">
						<div class="project-attachment-item">
							<h4 class="attachment-name">
								<span class="icon-note-paper-2"></span>
								<a target="_blank" href="https://github.com/rphlr/42-Subjects/">subject.pdf</a>
							</h4>
						</div>
						<div class="project-attachment-item">
							<h4 class="attachment-name">
								<span class="icon-note-paper-2"></span>
								<a target="_blank" href="https://github.com/rphlr/42-Subjects/">tester</a>
							</h4>
						</div>
						<div class="project-attachment-item">
							<h4 class="attachment-name">
								<span class="icon-note-paper-2"></span>
								<a target="_blank" href="https://github.com/rphlr/42-Subjects/">ubuntu_cgi_tester</a>
							</h4>
						</div>
						<div class="project-attachment-item">
							<h4 class="attachment-name">
								<span class="icon-note-paper-2"></span>
								<a target="_blank" href="https://github.com/rphlr/42-Subjects/">cgi_tester</a>
							</h4>
						</div>
						<div class="project-attachment-item">
							<h4 class="attachment-name">
								<span class="icon-note-paper-2"></span>
								<a target="_blank" href="https://github.com/rphlr/42-Subjects/">ubuntu_tester</a>
							</h4>
						</div>
					</div>

				</div>

			</div>
			<form class="simple_form form-horizontal" novalidate="novalidate" id="edit_scale_team_5494562"
				action="https://projects.intra.42.fr/projects/webserv/project_sessions/3318/evaluations/1647/scales/25879/scale_teams/5494562"
				accept-charset="UTF-8" method="post"><input name="utf8" type="hidden" value="✓"><input type="hidden"
					name="_method" value="patch"><input type="hidden" name="authenticity_token"
					value="Eb9lecFHrPZHFYq2n9rXsr8+FeMVPcdfjFzk89g0XrNDkqYVcQvmpm2GJX23YQkIP4Qq24r8PQtVvE0DVW2RdA==">
				<div class="scale-section-answers col-sm-12 scale-div">
					<section class="scale-section-item dancer from-down" id="section-72366">
						<div class="section-header">
							<h3 class="font-weight-bold mb-1">Mandatory Part</h3>
							<p class="font-italic"></p>
						</div>
						<h4 class="scale-question-name">Check the code and ask questions</h4>
						<p class="scale-section-guidelines">
						</p>
						<ul>
							<li>Launch the installation of siege with homebrew.</li>
							<li>Ask explanations about the basics of an HTTP server.</li>
							<li>Ask what function the group used for I/O Multiplexing.</li>
							<li>Ask for an explanation of how does select() (or equivalent) work.</li>
							<li>Ask if they use only one select() (or equivalent) and how they've
								managed the server to accept and the client to read/write.</li>
							<li>The select() (or equivalent) should be in the main loop and should
								check file descriptors for read and write AT THE SAME TIME. If not,
								the grade is 0 and the evaluation process ends now.</li>
							<li>There should be only one read or one write per client per select()
								(or equivalent). Ask the group to show you the code from the select()
								(or equivalent) to the read and write of a client.</li>
							<li>Search for all read/recv/write/send on a socket and check that, if
								an error is returned, the client is removed.</li>
							<li>Search for all read/recv/write/send and check if the returned value
								is correctly checked (checking only -1 or 0 values is not enough,
								both should be checked).</li>
							<li>If errno is checked after read/recv/write/send, the grade is 0 and
								the evaluation process ends now.</li>
							<li>Writing or reading ANY file descriptor without going through the
								select() (or equivalent) is strictly FORBIDDEN.</li>
							<li>The project must compile without any re-link issue. If not, use the
								'Invalid compilation' flag.</li>
							<li>If any point is unclear or is not correct, the evaluation stops.</li>
						</ul>

						<p></p>
						<div class="scale-question-answers">
							<div class="form-group hidden scale_team_answer_id">
								<div class="col-sm-10"><input class="form-control hidden"
										name="[scale_team][answers_attributes][0][id]" type="hidden"
										id="scale_team_answer_id"></div>
							</div>
							<div class="form-group hidden scale_team_answer_question_id">
								<div class="col-sm-10"><input class="form-control hidden" value="245747"
										name="[scale_team][answers_attributes][0][question_id]" type="hidden"
										id="scale_team_answer_question_id"></div>
							</div>
							<div class="btn-group block">
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][0][value]"
									id="radio_[scale_team][answers_attributes][0]_true" type="radio" value="1">
								<label class="btn btn-success checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][0]_true">
									<span class="icon-check"></span>
									Yes
								</label>
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][0][value]"
									id="radio_[scale_team][answers_attributes][0]_false" type="radio" value="0"
									checked="checked">
								<label class="btn btn-danger checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][0]_false">
									<span class="icon-times"></span>
									No
								</label>
							</div>
						</div>
						<h4 class="scale-question-name">Configuration</h4>
						<p class="scale-section-guidelines">
						</p>
						<p>In the configuration file, check whether you can do the following and <br>
							test the result:</p>

						<ul>
							<li>Search for the HTTP response status codes list on the internet.
								During this evaluation, if any status codes is wrong, don't give any
								related points.</li>
							<li>Setup multiple servers with different ports.</li>
							<li>Setup multiple servers with different hostnames (use something
								like: curl --resolve example.com:80:127.0.0.1 <a
									href="http://example.com/">http://example.com/</a>).
							</li>
							<li>Setup default error page (try to change the error 404).</li>
							<li>Limit the client body (use: curl -X POST -H "Content-Type: plain/text"
								--data "BODY IS HERE write something shorter or longer than body limit").</li>
							<li>Setup routes in a server to different directories.</li>
							<li>Setup a default file to search for if you ask for a directory.</li>
							<li>Setup a list of methods accepted for a certain route (e.g., try to
								delete something with and without permission).</li>
						</ul>

						<p></p>
						<div class="scale-question-answers">
							<div class="form-group hidden scale_team_answer_id">
								<div class="col-sm-10"><input class="form-control hidden"
										name="[scale_team][answers_attributes][1][id]" type="hidden"
										id="scale_team_answer_id"></div>
							</div>
							<div class="form-group hidden scale_team_answer_question_id">
								<div class="col-sm-10"><input class="form-control hidden" value="245748"
										name="[scale_team][answers_attributes][1][question_id]" type="hidden"
										id="scale_team_answer_question_id"></div>
							</div>
							<div class="btn-group block">
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][1][value]"
									id="radio_[scale_team][answers_attributes][1]_true" type="radio" value="1">
								<label class="btn btn-success checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][1]_true">
									<span class="icon-check"></span>
									Yes
								</label>
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][1][value]"
									id="radio_[scale_team][answers_attributes][1]_false" type="radio" value="0"
									checked="checked">
								<label class="btn btn-danger checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][1]_false">
									<span class="icon-times"></span>
									No
								</label>
							</div>
						</div>
						<h4 class="scale-question-name">Basic checks</h4>
						<p class="scale-section-guidelines">
						</p>
						<p>Using telnet, curl, prepared files, demonstrate that the following <br>
							features work properly:</p>

						<ul>
							<li>GET, POST and DELETE requests should work.</li>
							<li>UNKNOWN requests should not result in a crash.</li>
							<li>For every test you should receive the appropriate status code.</li>
							<li>Upload some file to the server and get it back.</li>
						</ul>

						<p></p>
						<div class="scale-question-answers">
							<div class="form-group hidden scale_team_answer_id">
								<div class="col-sm-10"><input class="form-control hidden"
										name="[scale_team][answers_attributes][2][id]" type="hidden"
										id="scale_team_answer_id"></div>
							</div>
							<div class="form-group hidden scale_team_answer_question_id">
								<div class="col-sm-10"><input class="form-control hidden" value="245749"
										name="[scale_team][answers_attributes][2][question_id]" type="hidden"
										id="scale_team_answer_question_id"></div>
							</div>
							<div class="btn-group block">
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][2][value]"
									id="radio_[scale_team][answers_attributes][2]_true" type="radio" value="1">
								<label class="btn btn-success checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][2]_true">
									<span class="icon-check"></span>
									Yes
								</label>
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][2][value]"
									id="radio_[scale_team][answers_attributes][2]_false" type="radio" value="0"
									checked="checked">
								<label class="btn btn-danger checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][2]_false">
									<span class="icon-times"></span>
									No
								</label>
							</div>
						</div>
						<h4 class="scale-question-name">Check CGI</h4>
						<p class="scale-section-guidelines">
						</p>
						<p>Pay attention to the following:</p>

						<ul>
							<li>The server is working fine using a CGI.</li>
							<li>The CGI should be run in the correct directory for relative path file access.</li>
							<li>With the help of the students you should check that everything is working properly.
								You have to test the CGI with the "GET" and "POST" methods.</li>
							<li>You need to test with files containing errors to see if the error handling works
								properly.
								You can use a script containing an infinite loop or an error; you are free to do
								whatever tests you want
								within the limits of acceptability that remain at your discretion.
								The group being evaluated should help you with this. </li>
						</ul>

						<p>The server should never crash and an error should be visible in case of a problem.</p>

						<p></p>
						<div class="scale-question-answers">
							<div class="form-group hidden scale_team_answer_id">
								<div class="col-sm-10"><input class="form-control hidden"
										name="[scale_team][answers_attributes][3][id]" type="hidden"
										id="scale_team_answer_id"></div>
							</div>
							<div class="form-group hidden scale_team_answer_question_id">
								<div class="col-sm-10"><input class="form-control hidden" value="245750"
										name="[scale_team][answers_attributes][3][question_id]" type="hidden"
										id="scale_team_answer_question_id"></div>
							</div>
							<div class="btn-group block">
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][3][value]"
									id="radio_[scale_team][answers_attributes][3]_true" type="radio" value="1">
								<label class="btn btn-success checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][3]_true">
									<span class="icon-check"></span>
									Yes
								</label>
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][3][value]"
									id="radio_[scale_team][answers_attributes][3]_false" type="radio" value="0"
									checked="checked">
								<label class="btn btn-danger checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][3]_false">
									<span class="icon-times"></span>
									No
								</label>
							</div>
						</div>
						<h4 class="scale-question-name">Check with a browser</h4>
						<p class="scale-section-guidelines">
						</p>
						<ul>
							<li>Use the reference browser of the team. Open the network part of it,
								and try to connect to the server using it.</li>
							<li>Look at the request header and response header.</li>
							<li>It should be compatible to serve a fully static website.</li>
							<li>Try a wrong URL on the server.</li>
							<li>Try to list a directory.</li>
							<li>Try a redirected URL.</li>
							<li>Try anything you want to.</li>
						</ul>

						<p></p>
						<div class="scale-question-answers">
							<div class="form-group hidden scale_team_answer_id">
								<div class="col-sm-10"><input class="form-control hidden"
										name="[scale_team][answers_attributes][4][id]" type="hidden"
										id="scale_team_answer_id"></div>
							</div>
							<div class="form-group hidden scale_team_answer_question_id">
								<div class="col-sm-10"><input class="form-control hidden" value="245751"
										name="[scale_team][answers_attributes][4][question_id]" type="hidden"
										id="scale_team_answer_question_id"></div>
							</div>
							<div class="btn-group block">
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][4][value]"
									id="radio_[scale_team][answers_attributes][4]_true" type="radio" value="1">
								<label class="btn btn-success checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][4]_true">
									<span class="icon-check"></span>
									Yes
								</label>
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][4][value]"
									id="radio_[scale_team][answers_attributes][4]_false" type="radio" value="0"
									checked="checked">
								<label class="btn btn-danger checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][4]_false">
									<span class="icon-times"></span>
									No
								</label>
							</div>
						</div>
						<h4 class="scale-question-name">Port issues</h4>
						<p class="scale-section-guidelines">
						</p>
						<ul>
							<li>In the configuration file setup multiple ports and use different
								websites. Use the browser to ensure that the configuration works as
								expected and shows the right website.</li>
							<li>In the configuration, try to setup the same port multiple times. It
								should not work.</li>
							<li>Launch multiple servers at the same time with different configurations
								but with common ports. Does it work? If it does, ask why the server
								should work if one of the configurations isn't functional. Keep going.</li>
						</ul>

						<p></p>
						<div class="scale-question-answers">
							<div class="form-group hidden scale_team_answer_id">
								<div class="col-sm-10"><input class="form-control hidden"
										name="[scale_team][answers_attributes][5][id]" type="hidden"
										id="scale_team_answer_id"></div>
							</div>
							<div class="form-group hidden scale_team_answer_question_id">
								<div class="col-sm-10"><input class="form-control hidden" value="245752"
										name="[scale_team][answers_attributes][5][question_id]" type="hidden"
										id="scale_team_answer_question_id"></div>
							</div>
							<div class="btn-group block">
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][5][value]"
									id="radio_[scale_team][answers_attributes][5]_true" type="radio" value="1">
								<label class="btn btn-success checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][5]_true">
									<span class="icon-check"></span>
									Yes
								</label>
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][5][value]"
									id="radio_[scale_team][answers_attributes][5]_false" type="radio" value="0"
									checked="checked">
								<label class="btn btn-danger checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][5]_false">
									<span class="icon-times"></span>
									No
								</label>
							</div>
						</div>
						<h4 class="scale-question-name">Siege &amp; stress test</h4>
						<p class="scale-section-guidelines">
						</p>
						<ul>
							<li>Use Siege to run some stress tests.</li>
							<li>Availability should be above 99.5% for a simple GET on an empty page
								with a siege -b on that page.</li>
							<li>Verify there is no memory leak (Monitor the process memory usage. It
								should not go up indefinitely).</li>
							<li>Check if there is no hanging connection.</li>
							<li>You should be able to use siege indefinitely without having to restart
								the server (take a look at siege -b).</li>
						</ul>

						<p></p>
						<div class="scale-question-answers">
							<div class="form-group hidden scale_team_answer_id">
								<div class="col-sm-10"><input class="form-control hidden"
										name="[scale_team][answers_attributes][6][id]" type="hidden"
										id="scale_team_answer_id"></div>
							</div>
							<div class="form-group hidden scale_team_answer_question_id">
								<div class="col-sm-10"><input class="form-control hidden" value="245753"
										name="[scale_team][answers_attributes][6][question_id]" type="hidden"
										id="scale_team_answer_question_id"></div>
							</div>
							<div class="btn-group block">
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][6][value]"
									id="radio_[scale_team][answers_attributes][6]_true" type="radio" value="1">
								<label class="btn btn-success checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][6]_true">
									<span class="icon-check"></span>
									Yes
								</label>
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][6][value]"
									id="radio_[scale_team][answers_attributes][6]_false" type="radio" value="0"
									checked="checked">
								<label class="btn btn-danger checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][6]_false">
									<span class="icon-times"></span>
									No
								</label>
							</div>
						</div>
					</section>
					<section class="scale-section-item dancer from-down" id="section-72367">
						<div class="section-header">
							<hr>
							<h3 class="font-weight-bold mb-1">Bonus part</h3>
							<p class="font-italic">Evaluate the bonus part if, and only if, the mandatory part has been
								entirely and perfectly done, and the error management handles unexpected
								or bad usage. In case all the mandatory points were not passed during the
								defense, bonus points must be totally ignored.
							</p>
						</div>
						<h4 class="scale-question-name">Cookies and session</h4>
						<p class="scale-section-guidelines">
						</p>
						<p>There is a working session and cookies system on the webserver.</p>

						<p></p>
						<div class="scale-question-answers">
							<div class="form-group hidden scale_team_answer_id">
								<div class="col-sm-10"><input class="form-control hidden"
										name="[scale_team][answers_attributes][7][id]" type="hidden"
										id="scale_team_answer_id"></div>
							</div>
							<div class="form-group hidden scale_team_answer_question_id">
								<div class="col-sm-10"><input class="form-control hidden" value="245754"
										name="[scale_team][answers_attributes][7][question_id]" type="hidden"
										id="scale_team_answer_question_id"></div>
							</div>
							<div class="btn-group block">
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][7][value]"
									id="radio_[scale_team][answers_attributes][7]_true" type="radio" value="1">
								<label class="btn btn-success checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][7]_true">
									<span class="icon-check"></span>
									Yes
								</label>
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][7][value]"
									id="radio_[scale_team][answers_attributes][7]_false" type="radio" value="0"
									checked="checked">
								<label class="btn btn-danger checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][7]_false">
									<span class="icon-times"></span>
									No
								</label>
							</div>
						</div>
						<h4 class="scale-question-name">CGI</h4>
						<p class="scale-section-guidelines">
						</p>
						<p>There is more than one CGI system.</p>

						<p></p>
						<div class="scale-question-answers">
							<div class="form-group hidden scale_team_answer_id">
								<div class="col-sm-10"><input class="form-control hidden"
										name="[scale_team][answers_attributes][8][id]" type="hidden"
										id="scale_team_answer_id"></div>
							</div>
							<div class="form-group hidden scale_team_answer_question_id">
								<div class="col-sm-10"><input class="form-control hidden" value="245755"
										name="[scale_team][answers_attributes][8][question_id]" type="hidden"
										id="scale_team_answer_question_id"></div>
							</div>
							<div class="btn-group block">
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][8][value]"
									id="radio_[scale_team][answers_attributes][8]_true" type="radio" value="1">
								<label class="btn btn-success checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][8]_true">
									<span class="icon-check"></span>
									Yes
								</label>
								<input autocomplete="off" class="checkbox-input"
									name="[scale_team][answers_attributes][8][value]"
									id="radio_[scale_team][answers_attributes][8]_false" type="radio" value="0"
									checked="checked">
								<label class="btn btn-danger checkbox-label light-bg"
									for="radio_[scale_team][answers_attributes][8]_false">
									<span class="icon-times"></span>
									No
								</label>
							</div>
						</div>
					</section>
				</div>
				<div class="col-sm-12 scale-div pb-4">
					<div class="dancer from-down" id="ratings-25879">
						<h3 class="font-weight-bold mb-1">Ratings</h3>
						<p class="marked-title">Don’t forget to check the flag corresponding to the defense</p>
						<div class="btn-group block flags-buttons" data-radio-item="flag-small">
							<label class="btn btn-default btn-success label_flag_ok on-active positive"
								for="scale_team_flag_id_1" name="scale_team[flag_id]">
								<span class="iconf-check-4"></span>
								Ok
							</label>
							<label class="btn btn-default btn-success label_flag_outstanding_project on-active positive"
								for="scale_team_flag_id_9" name="scale_team[flag_id]">
								<span class="iconf-star-1"></span>
								Outstanding project
							</label>
						</div>
						<div class="btn-group block flags-buttons" data-radio-item="flag-small">
							<label class="btn btn-danger btn-default label_flag_empty_work negative on-active"
								for="scale_team_flag_id_2" name="scale_team[flag_id]">
								<span class="iconf-iconf-folder-1"></span>
								Empty work
							</label>
							<label class="btn btn-danger btn-default label_flag_incomplete_work negative on-active"
								for="scale_team_flag_id_3" name="scale_team[flag_id]">
								<span class="iconf-file-attention"></span>
								Incomplete work
							</label>
							<label class="btn btn-danger btn-default label_flag_invalid_compilation negative on-active"
								for="scale_team_flag_id_5" name="scale_team[flag_id]">
								<span class="iconf-skull-2"></span>
								Invalid compilation
							</label>
							<label class="btn btn-danger btn-default label_flag_cheat negative on-active"
								for="scale_team_flag_id_7" name="scale_team[flag_id]">
								<span class="iconf-layers"></span>
								Cheat
							</label>
							<label class="btn btn-danger btn-default label_flag_crash negative on-active"
								for="scale_team_flag_id_8" name="scale_team[flag_id]">
								<span class="iconf-bomb"></span>
								Crash
							</label>
							<label class="btn btn-danger btn-default label_flag_incomplete_group negative on-active"
								for="scale_team_flag_id_10" name="scale_team[flag_id]">
								<span class="iconf-user-delete-2"></span>
								Incomplete group
							</label>
							<label class="btn btn-danger btn-default label_flag_concerning_situation negative on-active"
								for="scale_team_flag_id_11" name="scale_team[flag_id]">
								<span class="iconf-alert-2"></span>
								Concerning situation
							</label>
							<label class="btn btn-danger btn-default label_flag_leaks negative on-active"
								for="scale_team_flag_id_12" name="scale_team[flag_id]">
								<span class="iconf-blood"></span>
								Leaks
							</label>
							<label class="btn btn-danger btn-default label_flag_forbidden_function negative on-active"
								for="scale_team_flag_id_13" name="scale_team[flag_id]">
								<span class="iconf-delete-2"></span>
								Forbidden function
							</label>
							<label
								class="btn btn-danger btn-default label_flag_can’t_support_/_explain_code negative on-active"
								for="scale_team_flag_id_14" name="scale_team[flag_id]">
								<span class="iconf-bubble-attention-4"></span>
								Can’t support / explain code
							</label>
						</div>
					</div>

				</div>
				<div class="scale-final-submit"><a href="https://github.com/rphlr/42-Evals">
						<input class="btn btn-primary btn-block font-weight-bold" type="button"
							value="Give this repository a star. ⭐" /></a>
				</div>
			</form>
		</div>
	</div>
</body>

</html>