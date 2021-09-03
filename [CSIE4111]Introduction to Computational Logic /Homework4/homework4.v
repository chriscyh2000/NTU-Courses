Require Import Znumtheory .
Require Import Zdiv .
Require Import ZArith .
Import Z.

Section SimpleChineseRemainder .

Open Scope Z_scope .

Definition modulo (a b n : Z) : Prop := (n | (a - b)) .
Notation "( a == b [ n ])" := (modulo a b n) .

Lemma modulo_tran : forall a b c n : Z, 
    (a == b [ n ]) -> (b == c [ n ]) -> (a == c [ n ]) .
Proof .
  intros a b c n Hab Hbc .
  red in Hab, Hbc |- * .
  cut (a - c = a - b + (b - c)) .
  - intros H .
    rewrite H .
    apply Zdivide_plus_r .
    + trivial .
    + trivial .
  - auto with * .
Qed .

Lemma modulo_plus_subst : forall a b c n : Z,
    (a == b [ n ]) -> (a + c == b + c [ n ]) .
Proof .
(* to be done *)
  intros a b c n Hab .
  red in Hab |- * .
  cut (a + c - (b + c) = a - b) .
  - intros H .
    rewrite H .
    trivial .
  - auto with * .
Qed .

Lemma I_Love_BYWang :forall a b c m n : Z,
  (a * m == c [n]) <-> (a * m + b * n == c [n]) .
Proof .
  intros a b c m n .
  unfold iff .
  split .
  - intros H1 .
    red in H1 |- * .
    cut (a * m + b * n - c = a * m - c + b * n) .
    + intro Htrue .
      rewrite Htrue.
      apply Zdivide_plus_r.
      * trivial .
      * apply Zdivide_factor_l .
    + auto with * .
  - intro H2 .
    red in H2 |- * .
    apply divide_add_cancel_r with (m := b * n) .
    + apply Zdivide_factor_l .
    + cut (b * n + (a * m - c) = a * m + b * n - c) .
      * intros Htrue2 .
        rewrite Htrue2 .
        trivial .
      * auto with * .
Qed .
Lemma modulo_mult_subst : forall a b c n : Z,
    (a == b [ n ]) -> (a * c == b * c [ n ]) .
Proof .
(* to be done *)
  intros a b c n Hab .
  red in Hab |- * .
  cut (a * c - b * c = (a - b) * c) .
  - intros H .
    rewrite H .
    apply Zdivide_mult_l .
    trivial .
  - auto with * .    
Qed .

Hypothesis m n : Z .
Hypothesis co_prime : rel_prime m n .

Theorem modulo_inv : forall m n : Z, rel_prime m n ->
                       exists x : Z, (m * x == 1 [ n ]) .
Proof .
(* to be done *)
  intros m0 n0 .
  intros Hr .
  elim (Zis_gcd_bezout m0 n0 1) .
  intros u v H .
  - exists u .
    apply I_Love_BYWang with (b := v) .
    rewrite mul_comm .
    rewrite H .
    red .
    rewrite sub_diag .
    apply Zdivide_0 .
  - trivial .
Qed .

Theorem SimpleChineseRemainder : forall a b : Z,
  exists x : Z, (x == a [ m ]) /\ (x == b [ n ]) .
Proof .
(* to be done *)
  intros a b .
  destruct (rel_prime_bezout _ _ co_prime) as [u v H] .
  exists (a * v * n + b * u * m) .
  split .
  - apply add_move_l in H.
    rewrite <- mul_assoc .
    rewrite H .
    red .
    cut (a * (1 - u * m) + b * u * m - a = (b - a) * u * m) .
    + intros H1 .
      rewrite H1 .
      apply Zdivide_factor_l .
    + auto with *.
  - apply add_move_r in H.
    rewrite add_comm .
    rewrite <- mul_assoc .
    rewrite H .
    red .
    cut (b * (1 - v * n) + a * v * n - b = (a - b) * v * n) .
    + intros H2 .
      rewrite H2 .
      apply Zdivide_factor_l .
    + auto with *.
Qed .

End SimpleChineseRemainder .

Check SimpleChineseRemainder .