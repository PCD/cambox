(function ($) {
  Drupal.behaviors.camboxChat = {
    attach: function (context, settings) {
      $('#comments-raw', context).once('foo', function () {
        // Ajax Chat Form
        commentFormAjax();
        
        // Ajax Pull
        commentPullAjax();
        
        // Message Limit
        commentSetLimit();
        
        // Submit on Enter Key
        commentSubmitEnter();
        
        // Set Counter if Existing Pause
        commentCounterPause();
      });
    }
  };
  
  Drupal.behaviors.camboxChatApprove = {
    attach: function (context, settings) {
      $(document).on('click', '#comments-raw .comment li.approve a', function (event) {
        if ( approveActive == 0 ) {
          approveActive = 1;
          obj = $(this);
          url = $(this).attr('href');
          $.post(url, function(data){
            $(obj).parent().parent().parent().find('.submitted .unpublished_text').remove();
            $(obj).parent().parent().parent().removeClass('unpublished');
            $(obj).parent().remove();
            approveActive = 0;
          }, 'json');
        }
        event.preventDefault();
      });
    }
  };

/**
 * Some global vars.
 */
var pullActive = 0;
var pushActive = 0;
var chatActive = 1;
var approveActive = 0;

/**
 * Ajax Pull Chat.
 */
function commentPullAjax() {
  if ( pullActive == 0 ) {
    pullActive = 1;
    
    // Prepare Vars
    baseUrl = $('#comments-raw').attr('data-pull-url');
    lastCid = $('#comments-raw .comments .comment:first').attr('data-cid');
    baseUrl += lastCid;
    
    // Do Ajax Call
    $.post(baseUrl, function(data){
      if ( data.count > 0 ) {
        $('#comments-raw .comments .comment:first').removeClass('first');
        $('#comments-raw .comments').prepend(data.comments);
      }
      pullActive = 0;
      commentPullAjax();
    }, 'json');
  }
}

/**
 * Ajax Chat form.
 */
function commentFormAjax() {
  $('#cambox-chat-form').submit(function(){
    if ( pushActive == 0 && chatActive == 1 ) {
      pushActive = 1;
      url = $(this).attr('action');
      post = {message: $(this).find('.form-textarea').val()};
      $.post(url, post, function(data){
        $('#comments-raw .comments .comment.first').removeClass('first');
        $('#comments-raw .comments').prepend(data.comment);
        $('#cambox-chat-form .form-textarea').val('');
        commentSetLimitReset();
        pushActive = 0;
        commentCounter(0);
      }, 'json');
    }
    return false;
  });
}

/**
 * Submit on Enter Key.
 */
function commentSubmitEnter() {
  $('#cambox-chat-form textarea').keypress(function(e){
    if ( e.which == 13 ) {
      $('#cambox-chat-form').submit();
      e.preventDefault();
    }
  });
}

/**
 * Event for Limiting Text.
 */
function commentSetLimit() {
  $('#cambox-chat-form textarea').keyup(commentTextLimit).change(commentTextLimit).keydown(commentTextLimit);
}

/**
 * Text Limit
 */
function commentTextLimit() {
  textarea = $('#cambox-chat-form textarea');
  limit = parseInt($(textarea).attr('data-limit'));
  value = $(textarea).val();
  length = $(textarea).val().length;
  newvalue = value.substring(0, limit);
  $(textarea).val(newvalue);
  left = limit - newvalue.length;
  $('.message-limit').html(left);
}

/**
 * Reset Text Limit field.
 */
function commentSetLimitReset() {
  limit = parseInt($(textarea).attr('data-limit'));
  $('.message-limit').html(limit);
}

/**
 * Reset Time Wait.
 */
function commentTimeWaitReset() {
  pause = parseInt($('#cambox-chat-form textarea').attr('data-pause'));
  if ( pause > 0 ) {
    time_wait = pause;
  } else {
    time_wait = parseInt($('#cambox-chat-form textarea').attr('data-time-wait'));
  }
  $('#comment-form textarea').attr('placeholder', 'Espere ' + time_wait + ' segundos para volver a comentar.');
}

/**
 * Counter to Message again.
 */
function commentCounter(pause) {
  chatActive = 0;
  $('#comment-form').addClass('disabled');
  $('#comment-form textarea').attr('disabled', 'disabled');
  $('.comments-wrapper .comments').scrollTop(0);
  commentTimeWaitReset();
  if ( pause == 0 ) {
    time_wait = parseInt($(textarea).attr('data-time-wait'));
  } else {
    time_wait = pause;
  }
  var counter = 0;
  var interval = setInterval(function(){
    if ( pause > 0 ) {
      time_wait = pause;
      pause = 0;
    }
    counter++;
    left = time_wait - counter;
    //$('.time-left span.seconds').html(left);
    $('#comment-form textarea').attr('placeholder', 'Espere ' + left + ' segundos para volver a comentar.');
    if ( left == 0 ) {
      chatActive = 1;
      $('#comment-form').removeClass('disabled');
      $('#comment-form textarea').removeAttr('disabled');
      clearInterval(interval);
      $('#comment-form textarea').attr('placeholder', 'Escribe tu Mensaje');
    }
  }, 1000);
}

/**
 * Counter to Message again.
 */
function commentCounterPause() {
  pause = parseInt($('#cambox-chat-form textarea').attr('data-pause'), 10);
  if ( pause > 0 ) {
    commentCounter(pause);
  }
}

})(jQuery);