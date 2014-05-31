(function ($) {
  Drupal.behaviors.camboxChat = {
    attach: function (context, settings) {
      $('#comment-form', context).once('foo', function () {
        // Ajax Chat Form
        commentFormAjax();
        
        // Ajax Pull
        commentPullAjax();
      });
    }
  };

/**
 * Some global vars.
 */
var pullActive = 0;
var pushActive = 0;

/**
 * Ajax Pull Chat.
 */
function commentPullAjax() {
  if ( pullActive == 0 ) {
    pullActive = 1;
    
    // Prepare Vars
    baseUrl = $('#cambox-chat-form').attr('data-pull-url');
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
    if ( pushActive == 0 ) {
      pushActive = 1;
      url = $(this).attr('action');
      post = {message: $(this).find('.form-textarea').val()};
      $.post(url, post, function(data){
        $('#comments-raw .comments .comment.first').removeClass('first');
        $('#comments-raw .comments').prepend(data.comment);
        $('#cambox-chat-form .form-textarea').val('');
        pushActive = 0;
      }, 'json');
    }
    return false;
  });
}
})(jQuery);